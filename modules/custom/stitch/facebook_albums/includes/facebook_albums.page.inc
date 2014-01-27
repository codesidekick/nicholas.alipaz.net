<?php

/**
 * Page callback; does batch processing based on the access token provided.
 */
function facebook_albums_import_page($access_token) {
  if (!function_exists('fboauth_graph_query')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.fboauth');
  }
  if (!function_exists('facebook_albums_build_node')) {
    module_load_include('inc', 'facebook_albums', 'includes/facebook_albums.functions');
  }
  $fbuser = fboauth_graph_query('me', $access_token);
  $fbuid = fboauth_uid_load($fbuser->id);
  $fql = 'SELECT object_id, owner, cover_object_id, name, created, modified, description, photo_count, link, type FROM album WHERE owner="' . $fbuser->id . '"';
  $result = fboauth_graph_query('fql', $access_token, array('q' => $fql));
  $result->albums = array();
  $operations = array();
  foreach ($result->data as $album) {
    $album->access_token = $access_token;
    array_push($result->albums, $album->object_id);
    $operations[] = array('facebook_albums_process', array($album));
  }
  $operations[] = array('facebook_albums_cleanup', array($result->albums, $fbuid));
  $batch = array(
    'operations' => $operations,
    'title' => t('Processing Facebook Albums Batch'),
    'finished' => 'facebook_albums_finished',
    'init_message' => t('Import of albums is starting.'),
    'progress_message' => t('Processed @current out of @total albums.'),
    'error_message' => t('Batch processing has encountered an error.'),
    'file' => drupal_get_path('module', 'facebook_albums') . '/includes/facebook_albums.page.inc',
  );
  batch_set($batch);
  batch_process('user/' . $fbuid . '/facebook-albums');
}

/**
 * Batch API Operation callback; creates all the needed album nodes.
 * @param object $album
 * @param array $context
 */
function facebook_albums_process($album, &$context) {
  if (!function_exists('fboauth_graph_query')) {
    module_load_include('inc', 'fboauth', 'includes/fboauth.fboauth');
  }
  if (!function_exists('facebook_albums_build_node')) {
    module_load_include('inc', 'facebook_albums', 'includes/facebook_albums.functions');
  }
  $photo_fql = 'SELECT object_id, owner, src_big, link, caption, created, modified, position FROM photo WHERE album_object_id="' . $album->object_id . '" ORDER BY position';
  $photos = fboauth_graph_query('fql', $album->access_token, array('q' => $photo_fql))->data;
  
  // Create album node.
  $node = new stdClass();
  facebook_albums_build_node($node, $album, $photos, $context);
  $anode = (array) $node; // cast to array so we can check if empty.
  if (!empty($anode)) {
    node_save($node);
  }
}

/**
 * Batch API Operation callback; deletes any album nodes that had their album deleted on facebook.
 * @param array $albums
 */
function facebook_albums_cleanup($albums, $fbuid, &$context) {
  $sql = 'SELECT nid, field_facebook_id_value FROM {content_type_album} WHERE field_facebook_id_value IS NOT NULL';
  $results = db_query($sql);
  while ($result = db_fetch_object($results)) {
    $uid = db_result(db_query('SELECT uid FROM {node} WHERE nid = %s', $result->nid));
    if (!in_array($result->field_facebook_id_value, $albums) && $uid == $fbuid) {
      $node = node_load($node);
      node_delete($result->nid);
      $context['results'][] = t('Node "@title" with the facebook album id "@album_id" has been deleted.', array('@title' => $node->title, '@album_id' => $result->field_facebook_id_value));
    }
  }
}

function facebook_albums_finished($success, $results, $operations) {
  if ($success) {
    $message = implode('<br />', $results);
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}