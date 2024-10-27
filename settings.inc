<?php
// $Id$
function upgradation_settings_form($form, $form_state)
{
	$form['emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(Bcc) Notification emails'),
		'#description' => t('Specify emails id for Bcc option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('upgraded_case_study_emails', '')
	);
	$form['cc_emails'] = array(
		'#type' => 'textfield',
		'#title' => t('(Cc) Notification emails'),
		'#description' => t('Specify emails id for Cc option of mail system with comma separated'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('upgraded_case_study_cc_emails', '')
	);
	$form['from_email'] = array(
		'#type' => 'textfield',
		'#title' => t('Outgoing from email address'),
		'#description' => t('Email address to be display in the from field of all outgoing messages'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('upgraded_case_study_from_email', '')
	);
	// $form['extensions']['resource_upload'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t('Allowed file extensions for uploading resource files'),
	// 	'#description' => t('A comma separated list WITHOUT SPACE of source file extensions that are permitted to be uploaded on the server'),
	// 	'#size' => 50,
	// 	'#maxlength' => 255,
	// 	'#required' => TRUE,
	// 	'#default_value' => variable_get('resource_upload_extensions', '')
	// );
	// $form['extensions']['proposal_raw_data_file'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t('Allowed file extensions for uploading the raw data file in proposal form'),
	// 	'#description' => t('A comma separated list WITHOUT SPACE of file extensions that are permitted to be uploaded on the server'),
	// 	'#size' => 50,
	// 	'#maxlength' => 255,
	// 	'#required' => TRUE,
	// 	'#default_value' => variable_get('proposal_raw_data_upload_extensions', '')
	// );
	$form['extensions']['abstract_upload'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed file extensions for abstract'),
		'#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('upgradation_abstract_upload_extensions', '')
	);
	$form['extensions']['upload_case_study_developed_process'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed extensions for project files'),
		'#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('upgraded_case_study_project_files_extensions', '')
	);
	$form['extensions']['all_run'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed extensions for all run file'),
		'#description' => t('A comma separated list WITHOUT SPACE of pdf file extensions that are permitted to be uploaded on the server'),
		'#size' => 50,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('all_run_extensions', '')
	);
	// $form['extensions']['list_of_available_projects_file'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t('Allowed file extensions for file uploaded for available projects list'),
	// 	'#description' => t('A comma separated list WITHOUT SPACE of file extensions that are permitted to be uploaded on the server'),
	// 	'#size' => 50,
	// 	'#maxlength' => 255,
	// 	'#required' => TRUE,
	// 	'#default_value' => variable_get('list_of_available_projects_file', '')
	// );
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function upgradation_settings_form_validate($form, &$form_state)
{
	return;
}
function upgradation_settings_form_submit($form, &$form_state)
{
	variable_set('upgraded_case_study_emails', $form_state['values']['emails']);
	variable_set('upgraded_case_study_cc_emails', $form_state['values']['cc_emails']);
	variable_set('upgraded_case_study_from_email', $form_state['values']['from_email']);
	// variable_set('resource_upload_extensions', $form_state['values']['resource_upload']);
	// variable_set('proposal_raw_data_upload_extensions', $form_state['values']['proposal_raw_data_file']);
	variable_set('upgradation_abstract_upload_extensions', $form_state['values']['abstract_upload']);
	variable_set('upgraded_case_study_project_files_extensions', $form_state['values']['upload_case_study_developed_process']);
	variable_set('all_run_extensions', $form_state['values']['all_run']);
	// variable_set('list_of_available_projects_file', $form_state['values']['list_of_available_projects_file']);
	drupal_set_message(t('Settings updated'), 'status');
}
