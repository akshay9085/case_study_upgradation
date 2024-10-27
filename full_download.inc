<?php
// $Id$
function upgradation_download_full_project()
{
	global $user;
	$id = arg(3);
	$root_path = upgradation_path();
	// var_dump($root_path);die;
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('id', $id);
	$case_study_q = $query->execute();
	$case_study_data = $case_study_q->fetchObject();
	$query = db_select('csu_version', 'v')
    ->fields('v', array('id', 'case_study_version'));
    $query->innerJoin('case_study_proposal', 'p', 'p.version_id = v.id');
    $query->condition('v.id', $case_study_data->version_id);
    $version_data = $query->execute()->fetchObject();
    $version = $version_data->case_study_version;
    $simulation_type = $case_study_data->simulation_type_id;
	$version1 =  $version . '/';
    $simulation_type1 = $simulation_type . '/';
	$dest_path = $version1 . $simulation_type1;
	$CASE_STUDY_PATH = $dest_path . $case_study_data->directory_name . '/';
	/* zip filename */
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server */
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('id', $id);
	$circuit_simulation_udc_q = $query->execute();
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('id', $id);
	$query = db_select('csu_submitted_abstracts_file');
	$query->fields('csu_submitted_abstracts_file');
	$query->condition('proposal_id', $id);
	$project_files = $query->execute();
	while ($cfd_project_files = $project_files->fetchObject()) {
		$zip->addFile($root_path . $CASE_STUDY_PATH . $cfd_project_files->filepath, $CASE_STUDY_PATH . str_replace(' ', '_', basename($cfd_project_files->filename)));
	}
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file */
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $case_study_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $case_study_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are no case study project in this proposal to download", 'error');
		drupal_goto('case-study-upgradation/full-download/project');
	}
}
/*
function circuit_simulation_download_completed_project()
{
		global $user;
	$id = arg(3);
	$root_path = cfd_case_study_path();
	$query = db_select('case_study_proposal');
	$query->fields('case_study_proposal');
	$query->condition('id', $id);
	$case_study_q = $query->execute();
	$case_study_data = $case_study_q->fetchObject();
	$CASE_STUDY_PATH = $case_study_data->directory_name . '/';
	/* zip filename 
	$zip_filename = $root_path . 'zip-' . time() . '-' . rand(0, 999999) . '.zip';
	/* creating zip archive on the server 
	$zip = new ZipArchive();
	$zip->open($zip_filename, ZipArchive::CREATE);
	$query = db_select('case_study_proposal');
	$query->fields('case_study_proposal');
	$query->condition('id', $id);
	$circuit_simulation_udc_q = $query->execute();
	$query = db_select('case_study_proposal');
	$query->fields('case_study_proposal');
	$query->condition('id', $id);
	$query = db_select('case_study_submitted_abstracts_file');
	$query->fields('case_study_submitted_abstracts_file');
	$query->condition('proposal_id', $id);
	$project_files = $query->execute();
	//var_dump($root_path . $CASE_STUDY_PATH . 'project_files/');die;
	while ($cfd_project_files = $project_files->fetchObject()) {
		$zip->addFile($root_path . $CASE_STUDY_PATH . 'project_files/' . $cfd_project_files->filepath, $CASE_STUDY_PATH . str_replace(' ', '_', basename($cfd_project_files->filename)));
	}
	$zip_file_count = $zip->numFiles;
	$zip->close();
	if ($zip_file_count > 0) {
		if ($user->uid) {
			/* download zip file 
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $case_study_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		} //$user->uid
		else {
			header('Content-Type: application/zip');
			header('Content-disposition: attachment; filename="' . str_replace(' ', '_', $case_study_data->project_title) . '.zip"');
			header('Content-Length: ' . filesize($zip_filename));
			header("Content-Transfer-Encoding: binary");
			header('Expires: 0');
			header('Pragma: no-cache');
			ob_end_flush();
			ob_clean();
			flush();
			readfile($zip_filename);
			unlink($zip_filename);
		}
	} //$zip_file_count > 0
	else {
		drupal_set_message("There are circuit simulation project in this proposal to download", 'error');
		drupal_goto('circuit-simulation-project/full-download/project');
	}
}*/
