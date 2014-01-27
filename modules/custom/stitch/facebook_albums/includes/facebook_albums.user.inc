<?php

/**
 * Page callback; user page for the import.
 */
function facebook_albums_user_form() {
  return fboauth_action_display('facebook_albums_import');
}
