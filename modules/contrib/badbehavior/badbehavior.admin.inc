<?php

/**
 * @file
 * Admin page callbacks for the badbehavior module.
 */

function badbehavior_overview() {
  if (!badbehavior_load_includes(array('responses'))) {
    return 'Bad Behavior is not installed correctly.  Please download Bad Behavior and extract /bad-behavior/bad-behavior from the zip to sites/all/libraries/bad-behavior';
  }

  $header = array(
    array('data' => t('Response'), 'field' => 'w.http_response'),
    array('data' => t('Reason'), 'field' => 'w.denied_reason'),
    array('data' => t('Date'), 'field' => 'w.date', 'sort' => 'desc'),
    array('data' => t('IP'), 'field' => 'w.ip'),
    array('data' => t('Agent'), 'field' => 'w.user_agent', 'colspan' => 2)
  );
  if (variable_get('badbehavior_verbose_logging_enable', 0)) {
    $sql = 'SELECT w.* FROM {bad_behavior_log} w '. tablesort_sql($header);
  }
  else {
    $sql = "SELECT w.* FROM {bad_behavior_log} w WHERE w.key <> '00000000' " . tablesort_sql($header);
  }
  $result = pager_query($sql, 50);
  while ($behave = db_fetch_object($result)) {
    $response = bb2_get_response($behave->key);
    $behave->localdate = bb2_convertdate($behave->date);
    $rows[] = array('data' => array($response['response'], $response['log'], $behave->date, $behave->ip, $behave->user_agent, l(t('details'), "admin/reports/badbehavior/event/$behave->id")));
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No log messages available.'), 'colspan' => '6'));
  }

  $output = theme('table', $header, $rows) . theme('pager', NULL, 50, 0);

  return $output;
}

function badbehavior_event($id = NULL) {
  if (!badbehavior_load_includes(array('responses'))) {
    return 'Bad Behavior is not installed correctly.  Please download Bad Behavior and extract /bad-behavior/bad-behavior from the zip to sites/all/libraries/bad-behavior';
  }

  $output = '';
  $result = db_query('SELECT w.* FROM {bad_behavior_log} w WHERE w.id = %d', $id);
  if ($behave = db_fetch_object($result)) {
    $response = bb2_get_response($behave->key);
    $behave->localdate = bb2_convertdate($behave->date);
    $output .= '<table border="1" cellpadding="2" cellspacing="2">';
    $output .= ' <tr><th>'. t('IP Addr') .'</th><td>'. $behave->ip .'</td></tr>';
    $output .= ' <tr><th>'. t('Hostname') .'</th><td>'. gethostbyaddr($behave->ip) .' ('. l('whois', 'http://www.whois.sc/'. $behave->ip) .')</td></tr>';
    $output .= ' <tr><th>'. t('Date') .'</th><td>'. $behave->date .'</td></tr>';
    $output .= ' <tr><th>'. t('Request type') .'</th><td>'. $behave->request_method .'</td></tr>';
    $output .= ' <tr><th>'. t('URI') .'</th><td>'. $behave->request_uri .'</td></tr>';
    $output .= ' <tr><th>'. t('Protocol') .'</th><td>'. $behave->server_protocol .'</td></tr>';
    $output .= ' <tr><th>'. t('User Agent') .'</th><td>'. $behave->user_agent .'</td></tr>';
    $output .= ' <tr><th>'. t('Headers') .'</th><td>'. $behave->http_headers .'</td></tr>';
    $output .= ' <tr><th>'. t('Request Entity') .'</th><td>'. $behave->request_entity .'</td></tr>';
    $output .= ' <tr><th>'. t('Denied Reason') .'</th><td>'. $response['log'] .'</td></tr>';
    $output .= ' <tr><th>'. t('Explanation') .'</th><td>'. $response['explanation'] .'</td></tr>';
    $output .= ' <tr><th>'. t('Response') .'</th><td>'. $response['response'] .'</td></tr>';
    $output .= '</table>';
  }
  return $output;
}

function badbehavior_settings_form() {
  $form['badbehavior_mail'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator Email'),
    '#default_value' => variable_get('badbehavior_mail', variable_get('site_mail', ini_get('sendmail_from'))),
    '#description' => t('E-mail address for blocked users to contact in order to gain access.'),
    '#required' => TRUE,
  );
  $form['badbehavior_logging'] = array(
    '#type' => 'select',
    '#title' => t('Logging'),
    '#options' => array(
      0 => t('Disabled'),
      1 => t('Normal'),
      'verbose' => t('Verbose'),
    ),
    '#default_value' => variable_get('badbehavior_logging', 1),
  );
  $form['badbehavior_strict'] = array(
    '#type' => 'checkbox',
    '#title' => 'Enable strict mode',
    '#default_value' => variable_get('badbehavior_strict', 0),
    '#description' => t('Strict module blocks more spam but may cause false positives and block some people.'),
  );

  $form['httpbl'] = array(
    '#type' => 'fieldset',
    '#title' => t('http:BL'),
    '#description' => t("To use Bad Behavior's http:BL features you must have an <a href=\"@httpbl-url\">http:BL Access Key</a>", array('@httpbl-url' => 'http://www.projecthoneypot.org/httpbl_configure.php?rf=24694')),
  );
  $form['httpbl']['badbehavior_httpbl_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Your http:BL access key'),
    '#default_value' => variable_get('badbehavior_httpbl_key', ''),
    '#maxlength' => 12,
    '#size' => 12,
  );

  return system_settings_form($form);
}

function badbehavior_settings_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Validate the e-mail address.
  if (!valid_email_address($values['badbehavior_mail'])) {
    form_set_error('badbehavior_mail', t('The e-mail address %mail is not valid.', array('%mail' => $values['badbehavior_mail'])));
  }
}
