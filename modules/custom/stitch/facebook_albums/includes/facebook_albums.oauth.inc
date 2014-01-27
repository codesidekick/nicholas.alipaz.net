<?php

/**
 * Implements hook_fboauth_actions().
 */
function facebook_albums_fboauth_actions() {
  $actions['facebook_albums_import'] = array(
    'title' => t('Import my Facebook photos'),
    'callback' => 'facebook_albums_fboauth_action_import',
    'permissions' => array(
      'user_photos',
      'friends_photos',
    ),
//    'redirect_uri' => '/user',
  );
  return $actions;
}

/**
 * Facebook OAuth action callback; Import a user's Facebook albums and photos.
 */
function facebook_albums_fboauth_action_import($app_id, $access_token) {
  // Redirect to batch import.
  return 'facebook-albums/import/' . $access_token;
}
