<?php
function upgradation_run_form($form, &$form_state)
{
	$options_first = _list_of_upgraded_case_study();
	$url_case_study_id = (int) arg(2);
	// $url_case_study_id = (int) arg(3);
	$case_study_data = _upgraded_case_study_information($url_case_study_id);
	if ($case_study_data == 'Not found') {
		$url_case_study_id = '';
	} //$case_study_data == 'Not found'
	if (!$url_case_study_id) {
		$selected = isset($form_state['values']['case_study']) ? $form_state['values']['case_study'] : key($options_first);
	} //!$url_case_study_id
	elseif ($url_case_study_id == '') {
		$selected = 0;
	} //$url_case_study_id == ''
	else {
		$selected = $url_case_study_id;
	}
	// var_dump(_list_of_upgraded_case_study());die;
	$form = array();
	$form['case_study'] = array(
		'#type' => 'select',
		'#title' => t('Title of the case study'),
		'#options' => _list_of_upgraded_case_study(),
		'#default_value' => $selected,
		'#ajax' => array(
			'callback' => 'upgraded_case_study_project_details_callback'
		)
	);
	if (!$url_case_study_id) {
		$form['case_study_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_upgraded_case_study_details"></div>'
		);
		// $form['selected_case_study'] = array(
		// 	'#type' => 'item',
		// 	'#markup' => '<div id="ajax_selected_upgraded_case_study"></div>'
		// );
	} //!$url_case_study_id
	else {
		$cfd_case_study_id = $url_case_study_id;
		$case_study_default_value = $url_case_study_id;
		$form['case_study_details'] = array(
			'#type' => 'item',
			'#markup' => '<div id="ajax_upgraded_case_study_details">' . _upgraded_case_study_details($case_study_default_value) . '</div>'
		);
		// $form['selected_case_study'] = array(
		// 	'#type' => 'item',
		// 	'#markup' => '<div id="ajax_selected_upgraded_case_study">' . l('Download Abstract', "case-study-upgradation/download/project-file/" . $case_study_default_value) . '<br>' . l('Download Case Study', 'case-study-upgradation/full-download/project/' . $case_study_default_value) . '</div>'
		// );
	}
	return $form;
}
function upgraded_case_study_project_details_callback($form, $form_state)
{
	$commands = array();
	$case_study_default_value = $form_state['values']['case_study'];
	if ($case_study_default_value != 0) {
		$form['case_study_details']['#markup'] = _upgraded_case_study_details($case_study_default_value);
		$case_study_details = _upgraded_case_study_information($case_study_default_value);
		$provider = user_load($case_study_details->uid);
		if ($case_study_details->uid > 0) {
			$commands[] = ajax_command_html('#ajax_selected_upgraded_case_study', l('Download Abstract', "case-study-upgradation/download/project-file/" . $case_study_default_value) . '<br>' . l('Download case study', 'case-study-upgradation/full-download/project/' . $case_study_default_value));
		} //$case_study_details->uid > 0
		else {
			$commands[] = ajax_command_html('#ajax_selected_upgraded_case_study', '');
			$commands[] = ajax_command_html('#ajax_selected_upgraded_case_study', '');
		}
		$commands[] = ajax_command_html('#ajax_upgraded_case_study_details', _upgraded_case_study_details($case_study_default_value));
	} //$case_study_default_value != 0
	else {
		// $form['lab_experiment_list']['#options'] = _ajax_get_experiment_list();
		//  $commands[] = ajax_command_replace('#ajax_selected_experiment', drupal_render($form['lab_experiment_list']));
		$commands[] = ajax_command_html('#ajax_upgraded_case_study_details', '');
		$commands[] = ajax_command_html('#ajax_selected_upgraded_case_study', '');
		$commands[] = ajax_command_html('#ajax_selected_upgraded_case_study', '');
		$commands[] = ajax_command_data('#ajax_selected_upgraded_case_study', 'form_state_value_select', $form_state['values']['case_study']);
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
function bootstrap_table_format($headers, $rows)
{
	$thead = "";
	$tbody = "";
	foreach ($headers as $header) {
		$thead .= "<th>{$header}</th>";
	} //$headers as $header
	foreach ($rows as $row) {
		$tbody .= "<tr>";
		foreach ($row as $data) {
			$tbody .= "<td>{$data}</td>";
		} //$row as $data
		$tbody .= "</tr>";
	} //$rows as $row
	$table = "
            <table class='table table-bordered table-hover' style='margin-left:-140px'>
                <thead>{$thead}</thead>
                <tbody>{$tbody}</tbody>
            </table>
        ";
	return $table;
}
/*****************************************************/
function _list_of_upgraded_case_study()
{
	$case_study_titles = array(
		'0' => 'Please select...'
	);
	//$lab_titles_q = db_query("SELECT * FROM {case_study_proposal} WHERE solution_display = 1 ORDER BY lab_title ASC");
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('approval_status', 3);
	$query->orderBy('project_title', 'ASC');
	$case_study_titles_q = $query->execute();
	while ($case_study_titles_data = $case_study_titles_q->fetchObject()) {
		$case_study_titles[$case_study_titles_data->id] = $case_study_titles_data->project_title . ' (Proposed by ' . $case_study_titles_data->name_title . ' ' . $case_study_titles_data->contributor_name . ')';
	} //$case_study_titles_data = $case_study_titles_q->fetchObject()
	return $case_study_titles;
}
function _upgraded_case_study_information($proposal_id)
{
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('id', $proposal_id);
	$query->condition('approval_status', 3);
	$case_study_q = $query->execute();
	$case_study_data = $case_study_q->fetchObject();
	if ($case_study_data) {
		return $case_study_data;
	} //$case_study_data
	else {
		return 'Not found';
	}
}
function _upgraded_case_study_details($case_study_default_value)
{
	
	$case_study_details = _upgraded_case_study_information($case_study_default_value);
	if ($case_study_default_value != 0) {
		$query = db_select('csu_version', 'v')
    			->fields('v', array('id','case_study_version'));
		$query->condition('v.id', $case_study_details->version_id);
    	$version_data = $query->execute()->fetchObject();
		$version = $version_data->case_study_version;

		$query = db_select('case_study_software_version', 'v')
		->fields('v', array('id','case_study_version'));
$query->condition('v.id', $case_study_details->version_id);
$version_old_data = $query->execute()->fetchObject();
$old_version = $version_old_data->case_study_version;

		$query = db_select('case_study_proposal');
		$query->fields('case_study_proposal');
		$query->condition('project_title', $case_study_details->project_title);
		$old_case_study_q = $query->execute();
		$old_case_study_data = $old_case_study_q->fetchObject();
    // $cfd_case_study_id = $old_case_study_data->id;
	// var_dump($old_case_study_data);die;
		$form['case_study_details']['#markup'] = 
		'<table cellspacing="1" cellpadding="1" border="0" style="width: 100%;" valign="top">' . 
		'<tr><td style="width: 35%;"><span style="color: rgb(128, 0, 0);"><strong>About the Case Study</strong></span></td>
	<td style="width: 35%;"><span style="color: rgb(128, 0, 0);"><strong>Upgraded case study details</strong></span></td></tr>' .
	 '<tr><td valign="top">
	 <ul>' . '<li><strong>Title of the Case Study:</strong> ' . $case_study_details->project_title . '</li>' . 
	 '<li><strong>Contributor Name:</strong> ' . $old_case_study_data->name_title . ' ' . $old_case_study_data->contributor_name . '</li>' . 
	 '<li> <strong>Version:</strong> ' . $old_version . '</li>' . 
	 '<li><strong>University:</strong> ' . $old_case_study_data->university . '</li>' . '</ul></td><td valign="top">
	 <ul>' . '<li><strong>Proposer Name:</strong> ' . $case_study_details->name_title . ' ' . $case_study_details->contributor_name . '</li>' . '<li><strong>Upgraded Version:</strong> ' . $version . '</li>' . '<li><strong>University:</strong> ' . $case_study_details->university . '</li>' .   '</ul>'
		
		. '<tr><td style="width: 35%;">' . l('Download the report of the Case Study', "https://cfd.fossee.in/case-study-project/download/project-file/" . $old_case_study_data->id) . '<br>' . l('Download Case Files of the Original Case Study', 'https://cfd.fossee.in/case-study-project/full-download/project/' . $old_case_study_data->id) . '</td>
			<td style="width: 35%;">'  . l('Download Case Study', 'case-study-upgradation/full-download/project/' . $case_study_default_value)  . '</td></tr></table>';
		$details = $form['case_study_details']['#markup'];
		return $details;
		
	} //$case_study_default_value != 0
}
//. l('Download Abstract', "case-study-upgradation/download/project-file/" . $case_study_default_value) . '<br>'