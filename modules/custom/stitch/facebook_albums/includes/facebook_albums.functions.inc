<?php

/**
 * Utility function that processes each album to be imported.
 * @param object $node
 * @param object $album
 * @param object $photos
 */
function facebook_albums_build_node(&$node, $album, $photos, &$context) {
  $nid = db_result(db_query('SELECT nid FROM {content_type_album} WHERE field_facebook_id_value = %s', $album->object_id));
  $updated_photos = 0;
  $deleted_photos = 0;
  $created_photos = 0;
  // There is no node matching this facebook album id so create one.
  if (!is_string($nid)) {
    $action = t('created');
    facebook_albums_process_album_node($node, $album);
    $node->field_facebook_photos = facebook_albums_build_photo_node($photos);
    foreach ($photos as $photo) {
      $created_photos++;
      $node->field_photo[] = facebook_albums_save_file($photo, $album, $node);
    }
  }
  // Existing node should be updated.
  else {
    $action = t('checked');
    $last_update = db_result(db_query('SELECT changed FROM {node} WHERE nid = %s', $nid));
    $title = db_result(db_query('SELECT title FROM {node} WHERE nid = %s', $nid));
    if ($album->modified > $last_update) { // Only do the expense of a node_load if changes were made on fb.
      $action = t('modified');
      $new_photos = array();
      $node = node_load($nid);
      facebook_albums_process_album_node($node, $album);
      $node->field_photo = facebook_albums_process_album_photos($node, $photos, $updated_photos, $deleted_photos, $created_photos);
    }
  }
  $title = isset($node->title) ? $node->title : $title;
  $album_message = t('Album "!title" @action: ', array('!title' => $title, '@action' => $action));
  $number_message = t('@created photos created, @updated photos updated, @deleted photos deleted', array('@created' => $created_photos, '@updated' => $updated_photos, '@deleted' => $deleted_photos));
  $context['results'][] =  t('!album<br />@number.', array('!album' => $album_message, '@number' => $number_message));
  $context['message'] =  t('Album "!title" @action', array('!title' => $title, '@action' => $action));
}

function facebook_albums_build_photo_node($photos, $album, &$created_photos) {
  foreach ($photos as $photo) {
    $created_photos++;
    facebook_albums_process_photo_node(&$node, $photo, $album);
  }
  return $photos;
}

function facebook_albums_process_album_photos(&$node, $photos, &$updated_photos, &$deleted_photos, &$created_photos) {
  $ordered_photos = array();
  foreach ($photos as $k => $photo) {
    $photo->position--;
    $ordered_photos[$photo->object_id] = $photo;
  }
  foreach ($node->field_photo as $k => $photo_field) {
    $fb_photo_id = $photo_field['data']['fid']['body'];
    // Existing photo was modified on fb so update it.
    if (is_object($ordered_photos[$fb_photo_id]) && $ordered_photos[$fb_photo_id]->modified > $photo_field['timestamp']) {
      $updated_photos++;
      facebook_albums_delete_file($photo_field);
      $field_photo[$ordered_photos[$fb_photo_id]->position] = facebook_albums_save_file($ordered_photos[$fb_photo_id], $node);
      unset($ordered_photos[$fb_photo_id]);
    }
    // Existing photo not modified on fb but we should still correct its position.
    else if (is_object($ordered_photos[$fb_photo_id]) && $ordered_photos[$fb_photo_id]->modified <= $photo_field['timestamp']) {
      $field_photo[$ordered_photos[$fb_photo_id]->position] = $photo_field;
      unset($ordered_photos[$fb_photo_id]);
    }
    // Otherwise the photo was deleted from fb and we get rid of it in db and file system.
    else {
      $field_photo = NULL;
      $deleted_photos++;
      facebook_albums_delete_file($photo_field);
    }
  }
  // Any photos left in the array need to be saved and added to final values.
  foreach ($ordered_photos as $k => $photo) {
    $created_photos++;
    $field_photo[$photo->position] = facebook_albums_save_file($photo, $node);
  }
  return $field_photo;
}

/**
 * Utility function to create the base node object.
 * @param object $node
 * @param object $album
 * object_id, owner, src_big, link, caption, created, modified, position
 */
function facebook_albums_process_photo_node(&$node, $photo, $album) {
  $node->type = 'facebook_image';
  $node->uid = (string) fboauth_uid_load($album->owner);
  $node->status = '1';
  $node->created = $photo->created;
  $node->changed = $photo->modified;
  $node->title = $photo->object_id;
  $node->body = (isset($photo->caption)) ? $photo->caption : '';
  $node->teaser = $node->body;
  $node->format = '3';
  $node->field_facebook_id[0]['value'] = $photo->object_id;
  $node->field_facebook_url[0]['value'] = $photo->link;
  $node->field_photo[0]['value'] = $photo->link;
}

/**
 * Utility function to create the base node object.
 * @param object $node
 * @param object $album
 */
function facebook_albums_process_album_node(&$node, $album) {
  $node->type = 'album';
  $node->uid = (string) fboauth_uid_load($album->owner);
  $node->status = '1';
  $node->created = $album->created;
  $node->changed = $album->modified;
  $node->title = $album->name;
  $node->body = (isset($album->description)) ? $album->description : '';
  $node->teaser = $node->body;
  $node->format = '3';
  $node->field_facebook_id[0]['value'] = $album->object_id;
  $node->field_facebook_url[0]['value'] = $album->link;
  $node->field_facebook_cover_image_id[0]['value'] = $album->cover_object_id;
}

function facebook_albums_delete_file($file) {
  $success = file_delete($file['filepath']);
  if ($success) {
    db_query('DELETE FROM {files} WHERE fid = %d', $file['fid']);
  }
}

/**
 * Utility function to save a given remote file from url.
 * @param object $photo
 * @param object $node
 * @return array
 */
function facebook_albums_save_file($photo, $node) {
  $url = $photo->src_big;
  // Load remote image into a string and save to files directory.
  $request = drupal_http_request($url);
  if ($file_data = $request->data) {
    $filename = basename($url);
    $path = file_create_path('facebook-album');
    file_check_directory($path, FILE_CREATE_DIRECTORY);
    $filepath = $path . '/' . $filename;
    $filepath = file_save_data($file_data, $filepath);
    @chmod($filepath, 0664);
  }

  // Construct the file object.
  $file = new stdClass();
  $file->uid = $node->uid;
  $file->filename = $filename;
  $file->filepath = $filepath;
  $file->filemime = file_get_mimetype($filename);
  $file->filesize = filesize($filepath);
  $file->status = FILE_STATUS_PERMANENT;
  $file->timestamp = $photo->modified;
  // Save the file details to the {files} table.
  drupal_write_record('files', $file);
  // Get the fid from the db now that we saved the file.
  $file->fid = db_result(db_query("SELECT fid FROM {files} WHERE filepath = '%s'", $file->filepath));
  // Change some stuff and finish constructing the file object for the node.
  $file->list = '1';
  $file->data = array(
    'description' => $photo->caption,
    'alt' => basename($file->filename),
    'fid' => array(
      'body' => $photo->object_id,
    ),
  );
  return (array) $file;
}