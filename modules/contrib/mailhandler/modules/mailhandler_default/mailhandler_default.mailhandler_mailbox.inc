<?php
/**
 * @file
 * Default mailhandler mailbox for testing.
 */

/**
 * Implementation of hook_default_mailhandler_mailbox().
 */
function mailhandler_default_default_mailhandler_mailbox() {
  $export = array();

  $mailbox = new stdClass;
  $mailbox->disabled = FALSE; /* Edit this to true to make a default mailbox disabled initially */
  $mailbox->api_version = 2;
  $mailbox->mail = 'local_test';
  $mailbox->admin_title = 'Test mailbox';
  $mailbox->settings = array(
    'type' => 'local',
    'folder' => 'sites/all/modules/mailhandler/tests/test1_mbox',
    'domain' => 'dev.mailflow6.danepowell.com',
    'port' => '',
    'name' => '',
    'pass' => '',
    'extraimap' => '',
    'limit' => '0',
    'encoding' => 'UTF-8',
    'delete_after_read' => 0,
    'fromheader' => 'From',
    'security' => '0',
    'replies' => '0',
    'retrieve' => 'MailhandlerPhpImapRetrieve',
  );
  $export['local_test'] = $mailbox;

  return $export;
}
