<?php
/******************************************************************************/
/****************************** DELETION FUNCTIONS ****************************/
/******************************************************************************/
function upgradation_abstract_delete_project($proposal_id)
{
	$status = TRUE;
	$root_path = upgradation_path();
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('id', $proposal_id);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if (!$proposal_data)
	{
		drupal_set_message('Invalid Case Study Project.', 'error');
		return FALSE;
	} //!$proposal_data
	$query = db_select('csu_submitted_abstracts_file');
	$query->fields('csu_submitted_abstracts_file');
	$query->condition('proposal_id', $proposal_id);
	$abstract_q = $query->execute();
	$proposal_id = $proposal_data->id;
    $proposal_directory = $proposal_data->directory_name;
    /* create proposal folder if not present */
    //$dest_path = $proposal_directory . '/';
    $dest_path_project_files = $proposal_directory . '/';
    $query = db_select('csu_version', 'v')
    ->fields('v', array('id', 'case_study_version'));
    $query->innerJoin('case_study_proposal', 'p', 'p.version_id = v.id');
    $query->condition('v.id', $proposal_data->version_id);
    $version_data = $query->execute()->fetchObject();
    $version = $version_data->case_study_version;
    $simulation_type = $proposal_data->simulation_type_id;
    /* create proposal folder if not present */
    //$dest_path = $proposal_directory . '/';
    $dest_path_project_files = $proposal_directory . '/';
    $version1 =  $version . '/';
    $simulation_type1 = $simulation_type . '/';
	$dest_path = $version1 . $simulation_type1 .  $dest_path_project_files;
	$dir_project_files = $root_path . $dest_path;
	while ($abstract_data = $abstract_q->fetchObject())
	{
		if (is_dir($dir_project_files)){
// . '/project_files/' 
		unlink($root_path . $dest_path . $abstract_data->filepath);
		}
		else
		{
			drupal_set_message('Invalid case study project abstract.', 'error');
		}
		db_delete('csu_submitted_abstracts_file')->condition('proposal_id', $proposal_id)->execute();
		//!dwsim_flowsheet_delete_abstract_file($abstract_data->id)
	}
	// . '/project_files/'
	$res = rmdir($root_path . $dest_path);
	db_delete('csu_submitted_abstracts')->condition('proposal_id', $proposal_id)->execute();
	unlink($root_path . $dest_path . $proposal_data->samplefilepath);
	$res = rmdir($root_path . $dest_path);
	db_delete('csu_proposal')->condition('id', $proposal_data->id)->execute();
	return $status;
}