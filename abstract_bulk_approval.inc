<?php
// echo '<h1>20 hackathon submission on directory name is same or not</h1>';
function upgradation_abstract_bulk_approval_form($form, &$form_state)
{
	
	$options_first = _bulk_list_of_upgradation();
	$selected = isset($form_state['values']['case_study_project']) ? $form_state['values']['case_study_project'] : key($options_first);
	$form = array();
	
	$form['case_study_project'] = array(
		'#type' => 'select',
		'#title' => t('Title of the Case Study Project'),
		'#options' => _bulk_list_of_upgradation(),
		'#default_value' => $selected,
		'#ajax' => array(
			
			'callback' => 'ajax_bulk_upgradation_abstract_details_callback'
		),
		'#suffix' => '<div id="ajax_selected_case_study"></div><div id="ajax_selected_case_study_pdf"></div>'
	);
	$form['case_study_actions'] = array(
		'#type' => 'select',
		'#title' => t('Please select action for Case Study Project'),
		'#options' => _bulk_list_case_study_actions(),
		'#default_value' => 0,
		'#prefix' => '<div id="ajax_selected_case_study_action" style="color:red;">',
		'#suffix' => '</div>',
		'#states' => array(
			'invisible' => array(
				':input[name="case_study_project"]' => array(
					'value' => 0
				)
			)
		)
	);
	$form['message'] = array(
		'#type' => 'textarea',
		'#title' => t('Specify the reason for resubmission/ disapproval.'),
		'#prefix' => '<div id= "message_submit">',
		'#states' => array(
			'visible' => array(
				array(
					':input[name="case_study_actions"]' => array(
						'value' => 3
					)
				),
				'or',
				array(
					':input[name="case_study_actions"]' => array(
						'value' => 2
					)
				)
			)
		)
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#states' => array(
			'invisible' => array(
				':input[name="lab"]' => array(
					'value' => 0
				)
			)
		)
	);
	return $form;
}
function ajax_bulk_upgradation_abstract_details_callback($form, $form_state)
{
	$commands = array();
	$case_study_project_default_value = $form_state['values']['case_study_project'];
	if ($case_study_project_default_value != 0)
	{
		$commands[] = ajax_command_html('#ajax_selected_case_study', _case_study_details($case_study_project_default_value));
		$form['case_study_actions']['#options'] = _bulk_list_case_study_actions();
		$commands[] = ajax_command_replace('#ajax_selected_case_study_action', drupal_render($form['case_study_actions']));
	} //$case_study_project_default_value != 0
	else
	{
		$commands[] = ajax_command_html('#ajax_selected_case_study', '');
		$commands[] = ajax_command_data('#ajax_selected_case_study', 'form_state_value_select', $form_state['values']['case_study_project']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
/************************************************************/
function upgradation_abstract_bulk_approval_form_submit($form, &$form_state)
{
	global $user;
	$msg = '';
	$root_path = upgradation_path();
	//var_dump($root_path);die;
	if ($form_state['clicked_button']['#value'] == 'Submit')
	{
		if ($form_state['values']['case_study_project'])
			//var_dump($form_state['values']['case_study_actions']);die;
		// case_study_abstract_del_lab_pdf($form_state['values']['case_study_project']);
			if (user_access('Case Study bulk manage abstract'))
			{
				$query = db_select('csu_proposal');
				$query->fields('csu_proposal');
				$query->condition('id', $form_state['values']['case_study_project']);
				$user_query = $query->execute();
				$user_info = $user_query->fetchObject();
				//var_dump($user_info);die;
				$user_data = user_load($user_info->uid);
				if ($form_state['values']['case_study_actions'] == 1)
				{
					// approving entire project //
					$query = db_select('csu_submitted_abstracts_file');
					$query->fields('csu_submitted_abstracts_file');
					$query->condition('proposal_id', $form_state['values']['case_study_project']);
					$abstracts_q = $query->execute();
					// var_dump($abstracts_q);die;
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						db_query("UPDATE {csu_submitted_abstracts} SET abstract_approval_status = 1, is_submitted = 1, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						db_query("UPDATE {csu_submitted_abstracts_file} SET file_approval_status = 1, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					
					drupal_set_message(t('Approved Case Study Project. Use the checkbox below to publish this case study on the completed case studies page.'), 'status');
					drupal_goto('case-study-upgradation/manage-proposal/status/' . $form_state['values']['case_study_project']);
				} //$form_state['values']['case_study_actions'] == 1
				elseif ($form_state['values']['case_study_actions'] == 2)
				{
					//pending review entire project 
					$query = db_select('csu_submitted_abstracts_file');
					$query->fields('csu_submitted_abstracts_file');
					$query->condition('proposal_id', $form_state['values']['case_study_project']);
					$abstracts_q = $query->execute();
					$experiment_list = '';
					while ($abstract_data = $abstracts_q->fetchObject())
					{
						db_query("UPDATE {case_study_submitted_abstracts} SET abstract_approval_status = 0, is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->id
						));
						db_query("UPDATE {csu_proposal} SET is_submitted = 0, approver_uid = :approver_uid WHERE id = :id", array(
							':approver_uid' => $user->uid,
							':id' => $abstract_data->proposal_id
						));
						
						db_query("UPDATE {csu_submitted_abstracts_file} SET file_approval_status = 0, approvar_uid = :approver_uid WHERE submitted_abstract_id = :submitted_abstract_id", array(
							':approver_uid' => $user->uid,
							':submitted_abstract_id' => $abstract_data->id
						));
					} //$abstract_data = $abstracts_q->fetchObject()
					drupal_set_message(t('Resubmit the project files'), 'status');
					// email 
					$email_subject = t('[!site_name][Case Study Project] Your uploaded Case Study Project have been marked as pending', array(
						'!site_name' => variable_get('site_name', '')
					));
					$email_body = array(
						0 => t('

Dear ' . $user_info->contributor_name . ',

Kindly resubmit the project files for the project : ' . $user_info->project_title . '.

Reason for resubmission: ' . $form_state['values']['message'] . '
 

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
							'!site_name' => variable_get('site_name', ''),
							'!user_name' => $user_data->name
						))
					);
					/** sending email when everything done **/
					$email_to = $user_data->mail;
					$from = variable_get('upgraded_case_study_from_email', '');
					$bcc = variable_get('upgraded_case_study_emails', '');
					$cc = variable_get('upgraded_case_study_cc_emails', '');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('scilab_case_study', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						drupal_set_message('Error sending email message.', 'error');
					} //!drupal_mail('scilab_case_study', 'standard', $email_to, language_default(), $params, $from, TRUE)
				} //$form_state['values']['case_study_actions'] == 2
				elseif ($form_state['values']['case_study_actions'] == 3) //disapprove and delete entire Case Study Project
				{
					if (strlen(trim($form_state['values']['message'])) <= 30)
					{
						form_set_error('message', t(''));
						$msg = drupal_set_message("Please mention the reason for disapproval. Minimum 30 character required", 'error');
						return $msg;
					} //strlen(trim($form_state['values']['message'])) <= 30
					if (!user_access('Upgradation bulk delete abstract'))
					{
						$msg = drupal_set_message(t('You do not have permission to Bulk Dis-Approved and Deleted Entire Lab.'), 'error');
						return $msg;
					} //!user_access('case_study bulk delete code')
					if (upgradation_abstract_delete_project($form_state['values']['case_study_project'])) //////
					
					{
						drupal_set_message(t('Dis-Approved and Deleted Entire Case Study Project.'), 'status');
						$email_subject = t('[!site_name][Case Study Project] Your uploaded Case Study Project have been marked as dis-approved', array(
						'!site_name' => variable_get('site_name', '')
					));
					$email_body = array(
						0 => t('
Dear ' . $user_info->contributor_name . ',

Your uploaded Case Study Project files for the Case Study Project Title : ' . $user_info->project_title . ' have been marked as dis-approved.

Reason for dis-approval: ' . $form_state['values']['message'] . '

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
						'!site_name' => variable_get('site_name', ''),
						'!user_name' => $user_data->name
											))
					);
					$email_to = $user_data->mail;
					$from = variable_get('upgraded_case_study_from_email', '');
					$bcc = variable_get('upgraded_case_study_emails', '');
					$cc = variable_get('upgraded_case_study_cc_emails', '');
					$params['standard']['subject'] = $email_subject;
					$params['standard']['body'] = $email_body;
					$params['standard']['headers'] = array(
						'From' => $from,
						'MIME-Version' => '1.0',
						'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
						'Content-Transfer-Encoding' => '8Bit',
						'X-Mailer' => 'Drupal',
						'Cc' => $cc,
						'Bcc' => $bcc
					);
					if (!drupal_mail('scilab_case_study', 'standard', $email_to, language_default(), $params, $from, TRUE))
					{
						drupal_set_message('Error sending email message.', 'error');
					}
					} //case_study_abstract_delete_project($form_state['values']['case_study_project'])
					else
					{
						drupal_set_message(t('Error Dis-Approving and Deleting Entire Case Study Project.'), 'error');
					}
					// email 
					
				} //$form_state['values']['case_study_actions'] == 3
				
			} //user_access('case_study project bulk manage code')
		return $msg;
		
	} //$form_state['clicked_button']['#value'] == 'Submit'
}
/**********************************************************/
function _bulk_list_of_upgradation()
{
	$project_titles = array(
		'0' => 'Please select...'
	);
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('is_submitted', 1);
	$query->condition('approval_status', 1);
	$query->orderBy('project_title', 'ASC');
	$project_titles_q = $query->execute();
	while ($project_titles_data = $project_titles_q->fetchObject())
	{
		$project_titles[$project_titles_data->id] = $project_titles_data->project_title . ' (Proposed by ' . $project_titles_data->contributor_name . ')';
	} //$project_titles_data = $project_titles_q->fetchObject()
	return $project_titles;
}
function _bulk_list_case_study_actions()
{
	$case_study_actions = array(
		0 => 'Please select...'
	);
	$case_study_actions[1] = 'Approve Entire Case Study Project';
	$case_study_actions[2] = 'Resubmit Project files';
	$case_study_actions[3] = 'Dis-Approve Entire Case Study Project (This will delete Case Study Project)';
	//$case_study_actions[4] = 'Delete Entire Case Study Project Including Proposal';
	return $case_study_actions;
}
function _case_study_details($case_study_proposal_id)
{
	$return_html = "";
	$query_pro = db_select('csu_proposal');
	$query_pro->fields('csu_proposal');
	$query_pro->condition('id', $case_study_proposal_id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
	//var_dump($abstracts_pro);die;
	$query_pdf = db_select('csu_submitted_abstracts_file');
	$query_pdf->fields('csu_submitted_abstracts_file');
	$query_pdf->condition('proposal_id', $case_study_proposal_id);
	$query_pdf->condition('filetype', 'A');
	$abstracts_pdf = $query_pdf->execute()->fetchObject();
	if ($abstracts_pdf == TRUE)
	{
		if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "")
		{
			$abstract_filename = $abstracts_pdf->filename;
		} //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
		else
		{
			$abstract_filename = "File not uploaded";
		}
	} //$abstracts_pdf == TRUE
	else
	{
		$abstract_filename = "File not uploaded";
	}
	$query_process = db_select('csu_submitted_abstracts_file');
	$query_process->fields('csu_submitted_abstracts_file');
	$query_process->condition('proposal_id', $case_study_proposal_id);
	$query_process->condition('filetype', 'S');
	$abstracts_query_process = $query_process->execute()->fetchObject();
	$query = db_select('csu_submitted_abstracts');
	$query->fields('csu_submitted_abstracts');
	$query->condition('proposal_id', $case_study_proposal_id);
	$abstracts_q = $query->execute()->fetchObject();
	if ($abstracts_q)
	{
		if ($abstracts_q->is_submitted == 0)
		{
			//drupal_set_message(t('Abstract is not submmited yet.'), 'error', $repeat = FALSE);
			//return;
		} //$abstracts_q->is_submitted == 0
	} //$abstracts_q
	//var_dump($abstracts_query_process);die;
	if ($abstracts_query_process == TRUE)
	{
		if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "")
		{
			$abstracts_query_process_filename = $abstracts_query_process->filename;
		} //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
		else
		{
			$abstracts_query_process_filename = "File not uploaded";
		}
	} //$abstracts_query_process == TRUE
	else
	{
		$url = l('Upload abstract', 'case-study-upgradation/abstract-code/upload');
		$abstracts_query_process_filename = "File not uploaded";
	}
    $query_process2 = db_select('csu_submitted_abstracts_file');
	$query_process2->fields('csu_submitted_abstracts_file');
	$query_process2->condition('proposal_id', $case_study_proposal_id);
	$query_process2->condition('filetype', 'R');
	$abstracts_query_process2 = $query_process2->execute()->fetchObject();
	$query = db_select('csu_submitted_abstracts');
	$query->fields('csu_submitted_abstracts');
	$query->condition('proposal_id', $case_study_proposal_id);
	$abstracts_q2 = $query->execute()->fetchObject();
	if ($abstracts_q2)
	{
		if ($abstracts_q2->is_submitted == 0)
		{
			//drupal_set_message(t('Abstract is not submmited yet.'), 'error', $repeat = FALSE);
			//return;
		} //$abstracts_q->is_submitted == 0
	} //$abstracts_q
	//var_dump($abstracts_query_process);die;
	if ($abstracts_query_process2 == TRUE)
	{
		if ($abstracts_query_process2->filename != "NULL" || $abstracts_query_process2->filename != "")
		{
			$abstracts_query_process2_filename = $abstracts_query_process2->filename;
		} //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
		else
		{
			$abstracts_query_process2_filename = "File not uploaded";
		}
	} //$abstracts_query_process == TRUE
	else
	{
		$url = l('Upload abstract', 'case-study-upgradation/abstract-code/upload');
		$abstracts_query_process2_filename = "File not uploaded";
	}
	$download_case_study = l('Download Case Study Project','case-study-upgradation/full-download/project/'.$case_study_proposal_id);
	$return_html .= '<strong>Proposer Name:</strong><br />' . $abstracts_pro->name_title . ' ' . $abstracts_pro->contributor_name . '<br /><br />';
	$return_html .= '<strong>Title of the Case Study Project:</strong><br />' . $abstracts_pro->project_title . '<br /><br />';
	$return_html .= '<strong>Uploaded an abstract (brief outline) of the project:</strong><br />' . $abstract_filename . '<br /><br />';
	$return_html .= '<strong>Uploaded Case template Directory Folder:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
    $return_html .= '<strong>Uploaded Case solution Directory Folder:</strong><br />' . $abstracts_query_process2_filename . '<br /><br />';
	$return_html .= $download_case_study;
	return $return_html;
}