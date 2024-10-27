<?php
// a=report, s=source file  r=all run file
// $Id$
function upgradation_abstract()
{
    global $user;
    $return_html = "";
    $proposal_data = upgradation_get_proposal();
    if (!$proposal_data) {
        drupal_goto('');
        return;
    } //!$proposal_data
    //$return_html .= l('Upload abstract', 'case-study-project/abstract-code/upload') . '<br />';
    /* get experiment list */
    // $query = db_select('csu_proposals');
    // $query->fields('csu_proposal');
    // $query->condition('id', $proposal_data->id);
    
    $query = db_select('csu_submitted_abstracts');
    $query->fields('csu_submitted_abstracts');
    $query->condition('proposal_id', $proposal_data->id);
    $abstracts_q = $query->execute()->fetchObject();
    $query_pro = db_select('csu_proposal');
    $query_pro->fields('csu_proposal');
    $query_pro->condition('id', $proposal_data->id);
    $abstracts_pro = $query_pro->execute()->fetchObject();
    $query_pdf = db_select('csu_submitted_abstracts_file');
    $query_pdf->fields('csu_submitted_abstracts_file');
    $query_pdf->condition('proposal_id', $proposal_data->id);
    $query_pdf->condition('filetype', 'A');
    $abstracts_pdf = $query_pdf->execute()->fetchObject();
    if ($abstracts_pdf == true) {
        if ($abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != "") {
            $abstract_filename = $abstracts_pdf->filename;
            //$abstract_filename = l($abstracts_pdf->filename, 'case-study-project/download/project-file/' . $proposal_data->id);
        } //$abstracts_pdf->filename != "NULL" || $abstracts_pdf->filename != ""
        else {
            $abstract_filename = "File not uploaded";
        }
    } //$abstracts_pdf == TRUE
    else {
        $abstract_filename = "File not uploaded";
    }
    $query_process = db_select('csu_submitted_abstracts_file');
    $query_process->fields('csu_submitted_abstracts_file');
    $query_process->condition('proposal_id', $proposal_data->id);
    $query_process->condition('filetype', 'S');
    $abstracts_query_process = $query_process->execute()->fetchObject();
    if ($abstracts_query_process == true) {
        if ($abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != "") {
            $abstracts_query_process_filename = $abstracts_query_process->filename;
            //$abstracts_query_process_filename = l($abstracts_query_process->filename, 'case-study-project/download/project-file/' . $proposal_data->id);
        } //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
        else {
            $abstracts_query_process_filename = "File not uploaded";
        }
        if ($abstracts_q->is_submitted == '') {
            $url = l('Upload Case Directory', 'case-study-upgradation/abstract-code/upload');
        } //$abstracts_q->is_submitted == ''
        else if ($abstracts_q->is_submitted == 1) {
            $url = "";
        } //$abstracts_q->is_submitted == 1
        else if ($abstracts_q->is_submitted == 0) {
            $url = l('Edit', 'case-study-upgradation/abstract-code/upload');
        } //$abstracts_q->is_submitted == 0
    } //$abstracts_query_process == TRUE
    else {
        $url = l('Upload Case Directory', 'case-study-upgradation/abstract-code/upload');
        $abstracts_query_process_filename = "File not uploaded";
    }
    $query_process_r = db_select('csu_submitted_abstracts_file');
    $query_process_r->fields('csu_submitted_abstracts_file');
    $query_process_r->condition('proposal_id', $proposal_data->id);
    $query_process_r->condition('filetype', 'R');
    $abstracts_query_process_r = $query_process_r->execute()->fetchObject();
    if ($abstracts_query_process_r == true) {
        if ($abstracts_query_process_r->filename != "NULL" || $abstracts_query_process_r->filename != "") {
            $abstracts_query_process_r_filename = $abstracts_query_process_r->filename;
            //$abstracts_query_process_filename = l($abstracts_query_process->filename, 'case-study-project/download/project-file/' . $proposal_data->id);
        } //$abstracts_query_process->filename != "NULL" || $abstracts_query_process->filename != ""
        else {
            $abstracts_query_process_r_filename = "File not uploaded";
        }
        if ($abstracts_q->is_submitted == '') {
            $url = l('Upload Case Directory', 'case-study-upgradation/abstract-code/upload');
        } //$abstracts_q->is_submitted == ''
        else if ($abstracts_q->is_submitted == 1) {
            $url = "";
        } //$abstracts_q->is_submitted == 1
        else if ($abstracts_q->is_submitted == 0) {
            $url = l('Edit', 'case-study-upgradation/abstract-code/upload');
        } //$abstracts_q->is_submitted == 0
    } //$abstracts_query_process == TRUE
    else {
        $url = l('Upload Case Directory', 'case-study-upgradation/abstract-code/upload');
        $abstracts_query_process_r_filename = "File not uploaded";
    }
    $return_html .= '<strong>Contributor Name:</strong><br />' . $proposal_data->name_title . '. ' . $proposal_data->contributor_name . '<br /><br />';
    $return_html .= '<strong>Title of the Case Study Project:</strong><br />' . $proposal_data->project_title . '<br /><br />';
    $return_html .= '<strong>Uploaded abstract of the project:</strong><br />' . $abstract_filename . '<br /><br />';
    $return_html .= '<strong>Uploaded Case Directory:</strong><br />' . $abstracts_query_process_filename . '<br /><br />';
    $return_html .= '<strong>Uploaded All Run File:</strong><br />' . $abstracts_query_process_r_filename . '<br /><br />';
    $return_html .= $url . '<br />';
    return $return_html;
    
}
// check for all run script 
function upgradation_upload_abstract_code_form($form, &$form_state)
{
    global $user;
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data",
    );
    /* get current proposal */
    //$proposal_id = (int) arg(3);
    $uid = $user->uid;
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->condition('uid', $uid);
    $query->condition('approval_status', 1);
    $proposal_q = $query->execute();
    if ($proposal_q) {
        if ($proposal_data = $proposal_q->fetchObject()) {
            /* everything ok */
        } //$proposal_data = $proposal_q->fetchObject()
        else {
            drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
            drupal_goto('case-study-upgradation/abstract-code');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-upgradation/abstract-code');
        return;
    }
    
    $query = db_select('csu_submitted_abstracts');
    $query->fields('csu_submitted_abstracts');
    $query->condition('proposal_id', $proposal_data->id);
    $abstracts_q = $query->execute()->fetchObject();
    if ($abstracts_q) {
        if ($abstracts_q->is_submitted == 1) {
            drupal_set_message(t('You have already submited your Case Directory, hence you can not upload any more, for any query please write to us.'), 'error', $repeat = false);
            drupal_goto('case-study-upgradation/abstract-code');
            //return;
        } //$abstracts_q->is_submitted == 1
    } //$abstracts_q->is_submitted == 1
    $form['project_title'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->project_title,
        '#title' => t('Title of the Case Study Project'),
    );
    $form['contributor_name'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->contributor_name,
        '#title' => t('Contributor Name'),
    );
    // $form['version'] = array(
    //     '#type' => 'item',
    //     '#markup' => $proposal_data->version_id,
    //     '#title' => t('Version Used'),
    // );
   
    // $version_id = isset($form_state['values']['version']) ? $form_state['values']['version'] : NULL ;
    // var_dump($version_id );die;
	// if(!empty($version_id)){_csu_list_of_solvers($version_id)
	// $solver_options = _csu_list_of_solvers($version_id);
	$form['solver_used'] = array(
		'#type' => 'select',
		'#title' => t('Select the Solver to be used'),
		'#options' =>_csu_list_of_solvers($proposal_data->version_id) ,
		// '#prefix' => '<div id="ajax-solver-replace">',
		// '#suffix' => '</div>',
		'#default_value' =>  _csu_list_of_solvers($proposal_data->solver_used),
		// '#ajax' => array(
        //     // 'event'=>'change',
        //     'callback' =>'ajax_other_solver_callback',
        //     // 'wrapper' => 'dropdown-project-titles',
        // ),
		'#required' => TRUE,
		'#states' => array(
			'invisible' => array(
				':input[name="version"]' => array(
					'value' => ''
				)
			)
		),

	);
	// $solver_id = isset($form_state['values']['solver_used']) ? $form_state['values']['solver_used'] : _csu_list_of_solvers($version_id);
	// // }
	// $solvers = $form_state['values']['solver_used'];
	// //else if ($simulation_id == 19){
	// 	if($solvers == 'Other'){
	$form['solver_used_text'] = array(
		'#type' => 'textfield',
		'#title' => t("Enter the Solver to be used <span style='color:red;'>*</span>"),
		'#size' => 100,
		'#description' => t('Maximum character limit is 50'),
		// '#required' => TRUE,
		// '#prefix' => '<div id="ajax-solver-text-replace">',
		// '#suffix' => '</div>',
		// '#required' => TRUE,
		'#states' => array(
                'visible' => array(
                    ':input[name="solver_used"]' => array(
                        'value' => 'Other'
                    )
                )
            ),     
	);
    
    $existing_uploaded_A_file = default_value_for_uploaded_files("A", $proposal_data->id); if (!$existing_uploaded_A_file) {
        $existing_uploaded_A_file = new stdClass();
        $existing_uploaded_A_file->filename = "No file uploaded";
    }
    $form['upload_an_abstract'] = array(
		'#type' => 'file',
		'#title' => t('Upload an abstract of the project.') ,
		'#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('upgradation_abstract_upload_extensions', '') . '</span>'
	));
    $existing_uploaded_S_file = default_value_for_uploaded_files("S", $proposal_data->id);
    if (!$existing_uploaded_S_file) {
        $existing_uploaded_S_file = new stdClass();
        $existing_uploaded_S_file->filename = "No file uploaded";
    } //!$existing_uploaded_S_file
    $form['upload_case_study_developed_process'] = array(
        '#type' => 'file',
        '#title' => t('Upload the Case Directory'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_S_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('upgraded_case_study_project_files_extensions', '') . '</span>',
    );
    $existing_uploaded_R_file = default_value_for_uploaded_files("R", $proposal_data->id); if (!$existing_uploaded_R_file) {
        $existing_uploaded_R_file = new stdClass();
        $existing_uploaded_R_file->filename = "No file uploaded";
    }

    $form['all_run'] = array(
        '#type' => 'file',
        '#title' => t('Upload the All Run file'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_R_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('all_run_extensions', '') . '</span>',
    );
    $form['prop_id'] = array(
        '#type' => 'hidden',
        '#value' => $proposal_data->id,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#submit' => array(
            'upgradation_upload_abstract_code_form_submit',
        ),
    );
    $form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'case-study-upgradation/abstract-code'),
    );
    return $form;
}

///////////////////////////////////////////////////////////////
function upgradation_upload_abstract_code_form_validate($form, &$form_state)
{
    //var_dump($form);die;
    if (isset($_FILES['files'])) {
        /* check if atleast one source or result file is uploaded */
        if (!($_FILES['files']['name']['upload_case_study_developed_process'])) {
            form_set_error('upload_case_study_developed_process', t('Please upload the project files'));
        }
        if (!($_FILES['files']['name']['upload_an_abstract'])) {
            form_set_error('upload_an_abstract', t('Please upload the abstract file'));
        }
        if (!($_FILES['files']['name']['all_run'])) {
            form_set_error('all_run', t('Please upload the all run file'));
        }

        /* check for valid filename extensions */
        foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
            if ($file_name) {
                /* checking file type */
                if (strstr($file_form_name, 'upload_case_study_developed_process'))
                        $file_type = 'S';
                    else if (strstr($file_form_name, 'upload_an_abstract'))
                        $file_type = 'A';
                        else if (strstr($file_form_name, 'all_run'))
                        $file_type = 'R';
                    $allowed_extensions_str = '';
                    switch ($file_type)
                    {
                        case 'S':
                            $allowed_extensions_str = variable_get('upgraded_case_study_project_files_extensions', '');
                            break;
                        case 'A':
                            $allowed_extensions_str = variable_get('upgradation_abstract_upload_extensions', '');
                            break;
                        case 'R':
                            $allowed_extensions_str = variable_get('all_run_extensions', '');
                            break;
                    }
                $allowed_extensions = explode(',', $allowed_extensions_str);
                $fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
                $temp_extension = end($fnames);
                if (!in_array($temp_extension, $allowed_extensions)) {
                    form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
                }

                if ($_FILES['files']['size'][$file_form_name] <= 0) {
                    form_set_error($file_form_name, t('File size cannot be zero.'));
                }

                /* check if valid file name */
                if (!upgradation_check_valid_filename($_FILES['files']['name'][$file_form_name])) {
                    form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
                }

            } //$file_name
        } //$_FILES['files']['name'] as $file_form_name => $file_name
    }
if(empty($form_state['values']['solver_used'])){
        form_set_error('solver_used', t('Please select an option'));
    }

elseif($form_state['values']['solver_used'] == 'Other'){

if ($form_state['values']['solver_used_text'] != '')
{
    if (strlen($form_state['values']['solver_used_text']) > 100)
    {
        form_set_error('solver_used_text', t('Maximum charater limit is 100 charaters only, please check the length of the solver used'));
    } 
    else if (strlen($form_state['values']['solver_used_text']) < 7)
    {
        form_set_error('solver_used_text', t('Minimum charater limit is 7 charaters, please check the length of the solver used'));
    } 
} 

else
{
    form_set_error('solver_used_text', t('Solver used cannot be empty'));
}
}
    return $form_state;
}
function upgradation_upload_abstract_code_form_submit($form, &$form_state)
{
    global $user;
    $v = $form_state['values'];
    $root_path = upgradation_path();
    $proposal_data = upgradation_get_proposal();

    $proposal_id = $proposal_data->id;
    if (!$proposal_data) {
        drupal_goto('');
        return;
    } //!$proposal_data
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

    if (!is_dir($root_path . $version1))
		mkdir($root_path . $version1);

        if (!is_dir($root_path . $version1 . $simulation_type1))
		mkdir($root_path . $version1 . $simulation_type1);
    if (!is_dir($root_path . $version1 . $simulation_type1 . $dest_path_project_files))
    mkdir($root_path . $version1 . $simulation_type1 .  $dest_path_project_files);
 $dest_path = $version1 . $simulation_type1 .  $dest_path_project_files;
//  var_dump($dest_path);die;
    $proposal_id = $proposal_data->id;
    $query_s = "SELECT * FROM {csu_submitted_abstracts} WHERE proposal_id = :proposal_id";
    $args_s = array(
        ":proposal_id" => $proposal_id,
    );
    $query_s_result = db_query($query_s, $args_s)->fetchObject();
    if (!$query_s_result) {
        /* creating solution database entry */
        $query = "INSERT INTO {csu_submitted_abstracts} (
	proposal_id,
	approver_uid,
	abstract_approval_status,
	abstract_upload_date,
	abstract_approval_date,
	is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status,:abstract_upload_date, :abstract_approval_date, :is_submitted)";
        $args = array(
            ":proposal_id" => $proposal_id,
            ":approver_uid" => 0,
            ":abstract_approval_status" => 0,
            ":abstract_upload_date" => time(),
            ":abstract_approval_date" => 0,
            ":is_submitted" => 1,
        );
        $submitted_abstract_id = db_query($query, $args, array(
            'return' => Database::RETURN_INSERT_ID,
        ));
        $query1 = "UPDATE {csu_proposal} SET is_submitted = :is_submitted WHERE id = :id";
        $args1 = array(
            ":is_submitted" => 1,
            ":id" => $proposal_id,
        );
        db_query($query1, $args1);
        drupal_set_message('Abstract uploaded successfully.', 'status');
    } //!$query_s_result
    else {
        $query = "UPDATE {csu_submitted_abstracts} SET


	abstract_upload_date =:abstract_upload_date,
	is_submitted= :is_submitted
	WHERE proposal_id = :proposal_id
	";
        $args = array(

            ":abstract_upload_date" => time(),
            ":is_submitted" => 1,
            ":proposal_id" => $proposal_id,
        );
        $submitted_abstract_id = db_query($query, $args, array(
            'return' => Database::RETURN_INSERT_ID,
        ));
        $query1 = "UPDATE {csu_proposal} SET is_submitted = :is_submitted WHERE id = :id";
        $args1 = array(
            ":is_submitted" => 1,
            ":id" => $proposal_id,
        );
        db_query($query1, $args1);
        drupal_set_message('Abstract updated successfully.', 'status');
    }
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
        if ($file_name) {
            /* checking file type */
            if (strstr($file_form_name, 'upload_case_study_developed_process')) {
                $file_type = 'S';
            } //strstr($file_form_name, 'upload_case_study_developed_process')
            if (strstr($file_form_name, 'upload_an_abstract')) {
                $file_type = 'A';
            } //strstr($file_form_name, 'upload_case_study_developed_process')
            if (strstr($file_form_name, 'all_run')) {
                $file_type = 'R';
            } 
            // switch ($file_type) {
            //     case 'S':

                    if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])) {
                        //unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
                        drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
                            '!filename' => $_FILES['files']['name'][$file_form_name],
                        )), 'error');
                    } //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    /* uploading file */
                    else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])) {
                        /* for uploaded files making an entry in the database */
                        $query_abstracts = "SELECT * FROM csu_submitted_abstracts WHERE proposal_id = :proposal_id";
                        $query_abstracts_args = array(
                            ":proposal_id" => $proposal_id,
                        );
                        $query_abstracts_result = db_query($query_abstracts, $query_abstracts_args)->fetchObject();
                        $submitted_abstract_id = $query_abstracts_result->id;
                        $query_ab_f = "SELECT * FROM csu_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype =
				:filetype";
                        $args_ab_f = array(
                            ":proposal_id" => $proposal_id,
                            ":filetype" => $file_type,
                        );
                        $query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
                        if (!$query_ab_f_result) {
                            $query = "INSERT INTO {csu_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
          VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
                            $args = array(
                                ":submitted_abstract_id" => $submitted_abstract_id,
                                ":proposal_id" => $proposal_id,
                                ":uid" => $user->uid,
                                ":approvar_uid" => 0,
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":filetype" => $file_type,
                                ":timestamp" => time(),
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID));
                            drupal_set_message($file_name . ' uploaded successfully.', 'status');
                        } //!$query_ab_f_result
                        else {
                            unlink($root_path . $dest_path . $query_ab_f_result->filename);
                            $query = "UPDATE {csu_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
                            $args = array(
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":timestamp" => time(),
                                ":proposal_id" => $proposal_id,
                                ":filetype" => $file_type,
                            );
                            db_query($query, $args, array('return' => Database::RETURN_INSERT_ID,
                            ));

                            drupal_set_message($file_name . ' file updated successfully.', 'status');
                        }
                    } //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
                    else {
                        drupal_set_message('Error uploading file : ' . $dest_path . $file_name, 'error');
                    }
                             // } //$file_type
        } //$file_name
    } //$_FILES['files']['name'] as $file_form_name => $file_name

        if( $v['solver_used'] === 'Other'){
        	$solver = $v['solver_used_text'];
        }
        else{
            $solver = $v['solver_used'];
        }
        $result = "UPDATE {csu_proposal} SET solver_used=:solver_used";
        $args = array(
        ":solver_used" => $solver,
        );
        db_query($result, $args, array('return' => Database::RETURN_INSERT_ID,));
        drupal_set_message($file_name . ' solver updated successfully.', 'status');
    /* sending email */
    $email_to = $user->mail;
    $from = variable_get('cfd_hackathon_from_email', '');
    $bcc = variable_get('cfd_hackathon_emails', '');
    $cc = variable_get('cfd_hackathon_cc_emails', '');
    $params['abstract_uploaded']['proposal_id'] = $proposal_id;
    $params['abstract_uploaded']['submitted_abstract_id'] = $submitted_abstract_id;
    $params['abstract_uploaded']['user_id'] = $user->uid;
    $params['abstract_uploaded']['headers'] = array(
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc,
    );
    if (!drupal_mail('cfd_hackathon_submissions', 'abstract_uploaded', $email_to, language_default(), $params, $from, true)) {
        drupal_set_message('Error sending email message.', 'error');
    }

    drupal_goto('case-study-upgradation/abstract-code');
}
function default_value_for_uploaded_files($filetype, $proposal_id)
{
    $query = db_select('csu_submitted_abstracts_file');
    $query->fields('csu_submitted_abstracts_file');
    $query->condition('proposal_id', $proposal_id);
    $selected_files_array = "";
    if ($filetype == "S") {
        $query->condition('filetype', $filetype);
        $filetype_q = $query->execute()->fetchObject();
        return $filetype_q;
    } elseif ($filetype == "A") {
        $query->condition('filetype', $filetype);
        $filetype_q = $query->execute()->fetchObject();
        return $filetype_q;
    }
    elseif ($filetype == "R") {
        $query->condition('filetype', $filetype);
        $filetype_q = $query->execute()->fetchObject();
        return $filetype_q;
    }
    return;
}
function upgradation_edit_upload_abstract_code_form($form, &$form_state)
{
    global $user;
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data",
    );
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $uid = $user->uid;
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    if ($proposal_q) {
        if ($proposal_data = $proposal_q->fetchObject()) {
            /* everything ok */
        } //$proposal_data = $proposal_q->fetchObject()
        else {
            drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
            drupal_goto('case-study-upgradation/manage-proposal/edit-upload-file');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-upgradation/manage-proposal/edit-upload-file');
        return;
    }
    $query = db_select('csu_submitted_abstracts');
    $query->fields('csu_submitted_abstracts');
    $query->condition('proposal_id', $proposal_data->id);
    $abstracts_q = $query->execute()->fetchObject();
    $form['project_title'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->project_title,
        '#title' => t('Title of the Case Study Project'),
    );
    $form['contributor_name'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->contributor_name,
        '#title' => t('Contributor Name'),
    );
    $existing_uploaded_A_file = default_value_for_uploaded_files("A", $proposal_data->id); if (!$existing_uploaded_A_file) {
        $existing_uploaded_A_file = new stdClass();
        $existing_uploaded_A_file->filename = "No file uploaded";
    }
    $form['upload_an_abstract'] = array(
		'#type' => 'file',
		'#title' => t('Upload an abstract of the project.') ,
		'#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_A_file->filename . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('upgradation_abstract_upload_extensions', '') . '</span>'
	));
    $existing_uploaded_S_file = default_value_for_uploaded_files("S", $proposal_data->id);
    if (!$existing_uploaded_S_file) {
        $existing_uploaded_S_file = new stdClass();
        $existing_uploaded_S_file->filename = "No file uploaded";
    } //!$existing_uploaded_S_file
    $form['upload_case_study_developed_process'] = array(
        '#type' => 'file',
        '#title' => t('Upload the Case Directory'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_S_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('upgraded_case_study_project_files_extensions', '') . '</span>',
    );
    $existing_uploaded_R_file = default_value_for_uploaded_files("R", $proposal_data->id); if (!$existing_uploaded_R_file) {
        $existing_uploaded_R_file = new stdClass();
        $existing_uploaded_R_file->filename = "No file uploaded";
    }

    $form['all_run'] = array(
        '#type' => 'file',
        '#title' => t('Upload the All Run file'),
        //'#required' => TRUE,
        '#description' => t('<span style="color:red;">Current File :</span> ' . $existing_uploaded_R_file->filename . '<br />Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('all_run_extensions', '') . '</span>',
    );
    $form['prop_id'] = array(
        '#type' => 'hidden',
        '#value' => $proposal_data->id,
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#submit' => array(
            'upgradation_upload_abstract_code_form_submit',
        ),
    );
    $form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'case-study-project/manage-proposal/edit-upload-file'),
    );
    return $form;
}
///////////////////////////////////////////////////////////////
function upgradation_edit_upload_abstract_code_form_validate($form, &$form_state)
{
    if (!($_FILES['files']['name']['upload_an_abstract'] || $_FILES['files']['name']['upload_case_study_developed_process'] || $_FILES['files']['name']['all_run'])) {
        drupal_set_message('No files uploaded', 'error');
        return;
    }

    if (isset($_FILES['files'])) {
        /* check if atleast one source or result file is uploaded */
        if (!($_FILES['files']['name']['upload_case_study_developed_process'])) {
            form_set_error('upload_case_study_developed_process', t('Please upload the project files'));
        }
        if (!($_FILES['files']['name']['upload_an_abstract'])) {
            form_set_error('upload_an_abstract', t('Please upload the abstract file'));
        }
        if (!($_FILES['files']['name']['all_run'])) {
            form_set_error('all_run', t('Please upload the all run file'));
        }

        /* check for valid filename extensions */
        foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {
            if ($file_name) {
                /* checking file type */
                if (strstr($file_form_name, 'upload_case_study_developed_process'))
                        $file_type = 'S';
                    else if (strstr($file_form_name, 'upload_an_abstract'))
                        $file_type = 'A';
                        else if (strstr($file_form_name, 'all_run'))
                        $file_type = 'R';
                    $allowed_extensions_str = '';
                    switch ($file_type)
                    {
                        case 'S':
                            $allowed_extensions_str = variable_get('upgraded_case_study_project_files_extensions', '');
                            break;
                        case 'A':
                            $allowed_extensions_str = variable_get('upgradation_abstract_upload_extensions', '');
                            break;
                        case 'R':
                            $allowed_extensions_str = variable_get('all_run_extensions', '');
                            break;
                    }
                $allowed_extensions = explode(',', $allowed_extensions_str);
                $fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
                $temp_extension = end($fnames);
                if (!in_array($temp_extension, $allowed_extensions)) {
                    form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
                }

                if ($_FILES['files']['size'][$file_form_name] <= 0) {
                    form_set_error($file_form_name, t('File size cannot be zero.'));
                }

                /* check if valid file name */
                if (!upgradation_check_valid_filename($_FILES['files']['name'][$file_form_name])) {
                    form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
                }

            } //$file_name
        } //$_FILES['files']['name'] as $file_form_name => $file_name
    }
// if(empty($form_state['values']['solver_used'])){
//         form_set_error('solver_used', t('Please select an option'));
//     }

// elseif($form_state['values']['solver_used'] == 'Other'){

// if ($form_state['values']['solver_used_text'] != '')
// {
//     if (strlen($form_state['values']['solver_used_text']) > 100)
//     {
//         form_set_error('solver_used_text', t('Maximum charater limit is 100 charaters only, please check the length of the solver used'));
//     } 
//     else if (strlen($form_state['values']['solver_used_text']) < 7)
//     {
//         form_set_error('solver_used_text', t('Minimum charater limit is 7 charaters, please check the length of the solver used'));
//     } 
// } 

// else
// {
//     form_set_error('solver_used_text', t('Solver used cannot be empty'));
// }
// }
    return $form_state;
}
function upgradation_edit_upload_abstract_code_form_submit($form, &$form_state)
{
    global $user;
    $v = $form_state['values'];
    // version → simulation type → project_title → upload files

   
    $root_path = upgradation_path();
     $proposal_id = $proposal_data->id;
    if (!$proposal_data) {
        drupal_goto('');
        return;
    } //!$proposal_data
    // $proposal_id = $proposal_data->id;
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

    if (!is_dir($root_path . $version1))
		mkdir($root_path . $version1);

        if (!is_dir($root_path . $version1 . $simulation_type1))
		mkdir($root_path . $version1 . $simulation_type1);
    if (!is_dir($root_path . $version1 . $simulation_type1 . $dest_path_project_files))
    mkdir($root_path . $version1 . $simulation_type1 .  $dest_path_project_files);
 $dest_path = $version1 . $simulation_type1 .  $dest_path_project_files;
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->condition('id', $v['prop_id']);
    $proposal_q = $query->execute();
    $proposal_data = $proposal_q->fetchObject();
    $proposal_id = $proposal_data->id;
    if (!$proposal_data) {
        drupal_goto('');
        return;
    } //!$proposal_data
    $proposal_id = $proposal_data->id;
    $proposal_directory = $proposal_data->directory_name;
    
    $proposal_id = $proposal_data->id;
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name) {

        if ($file_name) {
            /* uploading file */
            /* checking file type */
            if (strstr($file_form_name, 'upload_case_study_abstract')) {
                $file_type = 'A';
                $abs_file_name = $_FILES['files']['name'][$file_form_name];
            } else {
                $abs_file_name = "Not updated";
            }
            if (strstr($file_form_name, 'upload_case_study_developed_process')) {
                $file_type = 'S';
                $proj_file_name = $_FILES['files']['name'][$file_form_name];
            } else {
                $proj_file_name = "Not updated";
            }
            if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name])) {
                $query_ab_f = "SELECT * FROM csu_submitted_abstracts_file WHERE proposal_id = :proposal_id AND filetype =
				:filetype";
                $args_ab_f = array(
                    ":proposal_id" => $proposal_id,
                    ":filetype" => $file_type,
                );
                $query_ab_f_result = db_query($query_ab_f, $args_ab_f)->fetchObject();
                unlink($root_path . $dest_path_project_files . $query_ab_f_result->filename);
                $query = "UPDATE {csu_submitted_abstracts_file} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id AND filetype = :filetype";
                $args = array(
                    ":filename" => $_FILES['files']['name'][$file_form_name],
                    ":filepath" => $file_path . $_FILES['files']['name'][$file_form_name],
                    ":filemime" => mime_content_type($root_path . $dest_path_project_files . $_FILES['files']['name'][$file_form_name]),
                    ":filesize" => $_FILES['files']['size'][$file_form_name],
                    ":timestamp" => time(),
                    ":proposal_id" => $proposal_id,
                    ":filetype" => $file_type,
                );
                db_query($query, $args, array('return' => Database::RETURN_INSERT_ID,
                ));

                drupal_set_message($file_name . ' file updated successfully.', 'status');

            } else {
                drupal_set_message($file_name . ' file not updated successfully.', 'error');
            }
        }
    } //$_FILES['files']['name'] as $file_form_name => $file_name
    /* sending email */
    $email_to = $user->mail;
    $from = variable_get('upgraded_case_study_from_email', '');
    $bcc = variable_get('upgraded_case_study_emails', '');
    $cc = variable_get('upgraded_case_study_cc_emails', '');
    $params['abstract_edit_file_uploaded']['proposal_id'] = $proposal_id;
    $params['abstract_edit_file_uploaded']['user_id'] = $user->uid;
    $params['abstract_edit_file_uploaded']['abs_file'] = $abs_file_name;
    $params['abstract_edit_file_uploaded']['proj_file'] = $proj_file_name;
    $params['abstract_edit_file_uploaded']['headers'] = array(
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc,
    );
    if (!drupal_mail('case_study', 'abstract_edit_file_uploaded', $email_to, language_default(), $params, $from, true)) {
        drupal_set_message('Error sending email message.', 'error');
    }
    drupal_goto('case-study-upgradation/abstract-code/edit-upload-files');
}
