<?php
// function upgradation_proposal_pending()
// {
//     /* get pending proposals to be approved */
//     $pending_rows = array();
//     $query = db_select('csu_proposal');
//     $query->fields('csu_proposal');
//     $query->condition('approval_status', 1);
//     $query->orderBy('id', 'DESC');
//     $pending_q = $query->execute();
//     while ($pending_data = $pending_q->fetchObject()) {
//         $pending_rows[$pending_data->id] = array(
//             date('d-m-Y', $pending_data->creation_date),
//             l($pending_data->name_title . ' ' . $pending_data->contributor_name, 'user/' . $pending_data->uid),
//             $pending_data->project_title,
//             l('Approve', 'case-study-upgradation/manage-proposal/approve/' . $pending_data->id) . ' | ' . l('Edit', 'case-study-upgradation/manage-proposal/edit/' . $pending_data->id),
//         );
//     } //$pending_data = $pending_q->fetchObject()
//     /* check if there are any pending proposals */
//     if (!$pending_rows) {
//         drupal_set_message(t('There are no pending proposals.'), 'status');
//         return '';
//     } //!$pending_rows
//     $pending_header = array(
//         'Date of Submission',
//         'Student Name',
//         'Title of the Case Study Project',
//         'Action',
//     );
//     //$output = theme_table($pending_header, $pending_rows);
//     $output = theme('table', array(
//         'header' => $pending_header,
//         'rows' => $pending_rows,
//     ));
//     return $output;
// }
function upgradation_proposal_all()
{
    /* get pending proposals to be approved */
    $proposal_rows = array();
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->orderBy('id', 'DESC');
    $proposal_q = $query->execute();
    while ($proposal_data = $proposal_q->fetchObject()) {
        $approval_status = '';
        switch ($proposal_data->approval_status) {
            case 0:
                $approval_status = 'Pending';
                break;
            case 1:
                $approval_status = 'Approved';
                break;
            case 2:
                $approval_status = 'Dis-approved';
                break;
            case 3:
                $approval_status = 'Completed';
                break;
            case 5:
                $approval_status = 'On Hold';
                break;
            default:
                $approval_status = 'Unknown';
                break;
        } //$proposal_data->approval_status
        if ($proposal_data->actual_completion_date == 0) {
            $actual_completion_date = "Not Completed";
        } //$proposal_data->actual_completion_date == 0
        else {
            $actual_completion_date = date('d-m-Y', $proposal_data->actual_completion_date);
        }
        if ($proposal_data->approval_date == 0) {
            $approval_date = "Not Approved";
        } //$proposal_data->actual_completion_date == 0
        else {
            $approval_date = date('d-m-Y', $proposal_data->approval_date);
        }
        $proposal_rows[] = array(
            date('d-m-Y', $proposal_data->creation_date),
            l($proposal_data->contributor_name, 'user/' . $proposal_data->uid),
            $proposal_data->project_title,
            $approval_date,
            $actual_completion_date,
            $approval_status,
            l('Status', 'case-study-upgradation/manage-proposal/status/' . $proposal_data->id) . ' | ' . l('Edit', 'case-study-upgradation/manage-proposal/edit/' . $proposal_data->id),
        );
    } //$proposal_data = $proposal_q->fetchObject()
    /* check if there are any pending proposals */
    if (!$proposal_rows) {
        drupal_set_message(t('There are no proposals.'), 'status');
        return '';
    } //!$proposal_rows
    $proposal_header = array(
        'Date of Submission',
        'Student Name',
        'Title of the case-study project',
        'Date of Approval',
        'Date of Project Completion',
        'Status',
        'Action',
    );
    $output = theme('table', array(
        'header' => $proposal_header,
        'rows' => $proposal_rows,
    ));
    return $output;
}
function upgradation_proposal_edit_file_all()
{
    /* get pending proposals to be approved */
    $proposal_rows = array();
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->orderBy('id', 'DESC');
    // $query->condition('approval_status', '0', '<>');
    $query->condition('approval_status', '1', '<>');
    $query->condition('approval_status', '2', '<>');
    $query->orderBy('approval_status', 'DESC');
    $proposal_q = $query->execute();
    while ($proposal_data = $proposal_q->fetchObject()) {
        $approval_status = '';
        switch ($proposal_data->approval_status) {
            case 0:
                $approval_status = 'Pending';
                break;
            case 1:
                $approval_status = 'Approved';
                break;
            case 2:
                $approval_status = 'Dis-approved';
                break;
            case 3:
                $approval_status = 'Completed';
                break;
            case 5:
                $approval_status = 'On Hold';
                break;
            default:
                $approval_status = 'Unknown';
                break;
        } //$proposal_data->approval_status
        if ($proposal_data->actual_completion_date == 0) {
            $actual_completion_date = "Not Completed";
        } //$proposal_data->actual_completion_date == 0
        else {
            $actual_completion_date = date('d-m-Y', $proposal_data->actual_completion_date);
        }
        if ($proposal_data->approval_date == 0) {
            $approval_date = "Not Approved";
        } //$proposal_data->actual_completion_date == 0
        else {
            $approval_date = date('d-m-Y', $proposal_data->approval_date);
        }
        $proposal_rows[] = array(
            date('d-m-Y', $proposal_data->creation_date),
            l($proposal_data->contributor_name, 'user/' . $proposal_data->uid),
            $proposal_data->project_title,
            $approval_date,
            $actual_completion_date,
            $approval_status,
            l('Edit', 'case-study-upgradation/abstract-code/edit-upload-files/' . $proposal_data->id),
        );
    } //$proposal_data = $proposal_q->fetchObject()
    /* check if there are any pending proposals */
    if (!$proposal_rows) {
        drupal_set_message(t('There are no proposals.'), 'status');
        return '';
    } //!$proposal_rows
    $proposal_header = array(
        'Date of Submission',
        'Student Name',
        'Title of the case-study project',
        'Date of Approval',
        'Date of Project Completion',
        'Status',
        'Action',
    );
    $output = theme('table', array(
        'header' => $proposal_header,
        'rows' => $proposal_rows,
    ));
    return $output;
}
/******************************************************************************/
/*************************** PROPOSAL STATUS FORM *****************************/
/******************************************************************************/
function upgradation_proposal_status_form($form, &$form_state)
{
    global $user;
    /* get current proposal */
    $proposal_id = (int) arg(3);
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    $query_abstract = db_select('csu_submitted_abstracts_file');
    $query_abstract->fields('csu_submitted_abstracts_file');
    $query_abstract->condition('proposal_id', $proposal_id);
    $query_abstract->condition('filetype', 'A');
   
    $query_abstract_pdf = $query_abstract->execute()->fetchObject();
    if ($proposal_q) {
        if ($proposal_data = $proposal_q->fetchObject()) {
            /* everything ok */
        } //$proposal_data = $proposal_q->fetchObject()
        else {
            drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
            drupal_goto('case-study-upgradation/manage-proposal');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-project/manage-proposal');
        return;
    }
    if ($proposal_data->faculty_name == '') {
        $faculty_name = 'NA';
    } else {
        $faculty_name = $proposal_data->faculty_name;
    }
    if ($proposal_data->faculty_department == '') {
        $faculty_department = 'NA';
    } else {
        $faculty_department = $proposal_data->faculty_department;
    }
    if ($proposal_data->faculty_email == '') {
        $faculty_email = 'NA';
    } else {
        $faculty_email = $proposal_data->faculty_email;
    }
    $query = db_select('csu_version', 'v')
    ->fields('v', array('id', 'case_study_version'));
    $query->innerJoin('case_study_proposal', 'p', 'p.version_id = v.id');
    $query->condition('v.id', $proposal_data->version_id);
    $version_data = $query->execute()->fetchObject();
    if(!$version_data){
        $version = 'NA';
    }
    else{
    $version = $version_data->case_study_version;
    }
    // $query = db_select('case_study_simulation_type');
    // $query->fields('case_study_simulation_type');
    // $query->condition('id', $proposal_data->simulation_type_id);
    // $simulation_type_data = $query->execute()->fetchObject();
    // if(!$simulation_type_data){
    //     $simulation_type = 'NA';
    // }
    // else{
    // $simulation_type = $simulation_type_data->simulation_type;
    // }
    $form['contributor_name'] = array(
        '#type' => 'item',
        '#markup' => l($proposal_data->name_title . ' ' . $proposal_data->contributor_name, 'user/' . $proposal_data->uid),
        '#title' => t('Student name'),
    );
    $form['student_email_id'] = array(
        '#title' => t('Student Email'),
        '#type' => 'item',
        '#markup' => user_load($proposal_data->uid)->mail,
        '#title' => t('Email'),
    );
    $form['university'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->university,
        '#title' => t('University/Institute'),
    );
    $form['how_did_you_know_about_project'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->how_did_you_know_about_project,
        '#title' => t('How did you know about the project'),
    );
    $form['faculty_name'] = array(
        '#type' => 'item',
        '#markup' => $faculty_name,
        '#title' => t('Name of the faculty'),
    );
    $form['faculty_department'] = array(
        '#type' => 'item',
        '#markup' => $faculty_department,
        '#title' => t('Department of the faculty'),
    );
    $form['faculty_email'] = array(
        '#type' => 'item',
        '#markup' => $faculty_email,
        '#title' => t('Email of the faculty'),
    );
    $form['country'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->country,
        '#title' => t('Country'),
    );
    $form['all_state'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->state,
        '#title' => t('State'),
    );
    $form['city'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->city,
        '#title' => t('City'),
    );
    $form['pincode'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->pincode,
        '#title' => t('Pincode/Postal code'),
    );
    $form['project_title'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->project_title,
        '#title' => t('Title of the Case Study Project'),
    );
    $form['version'] = array(
        '#type' => 'item',
        '#markup' => $version,
        '#title' => t('Version used'),
    );
    $form['simulation_type'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->simulation_type_id,
        '#title' => t('Simulation Type'),
    );
    $form['solver_used'] = array(
        '#type' => 'item',
        '#markup' => $proposal_data->solver_used,
        '#title' => t('Solver used'),
    );
    /************************** reference link filter *******************/
    $url = '~(?:(https?)://([^\s<]+)|(www\.[^\s<]+?\.[^\s<]+))(?<![\.,:])~i';
    $reference = preg_replace($url, '<a href="$0" target="_blank" title="$0">$0</a>', $proposal_data->reference);
    /******************************/
    /*$form['reference'] = array(
    '#type' => 'item',
    '#markup' => $reference,
    '#title' => t('References')
    );*/
    // if (($query_abstract_pdf->filename != "") && ($query_abstract_pdf->filename != 'NULL')) {
    //     $str = substr($query_abstract_pdf->filename, strrpos($query_abstract_pdf->filename, '/'));
    //     $resource_file = ltrim($str, '/');

    //     $form['abstract_file_path'] = array(
    //         '#type' => 'item',
    //         '#title' => t('Abstract file '),
    //         '#markup' => l($resource_file, 'case-study-upgradation/download/project-file/' . $proposal_id) . "",
    //     );
    // } //$proposal_data->user_defined_compound_filepath != ""
    // else {
    //     $form['abstract_file_path'] = array(
    //         '#type' => 'item',
    //         '#title' => t('Abstract file '),
    //         '#markup' => "Not uploaded<br><br>",
    //     );
    // }
    $proposal_status = '';
    switch ($proposal_data->approval_status) {
        case 0:
            $proposal_status = t('Pending');
            break;
        case 1:
            $proposal_status = t('Approved');
            break;
        case 2:
            $proposal_status = t('Dis-approved');
            break;
        case 3:
            $proposal_status = t('Completed');
            break;
        case 5:
                $approval_status = t('On Hold');
                break;
        default:
            $proposal_status = t('Unkown');
            break;
    }
    $form['proposal_status'] = array(
        '#type' => 'item',
        '#markup' => $proposal_status,
        '#title' => t('Proposal Status'),
    );
    if ($proposal_data->approval_status == 0) {
        $form['approve'] = array(
            '#type' => 'item',
            '#markup' => l('Click here', 'case-study-project/manage-proposal/approve/' . $proposal_id),
            '#title' => t('Approve'),
        );
    } //$proposal_data->approval_status == 0
    if ($proposal_data->approval_status == 1) {
        $form['completed'] = array(
            '#type' => 'checkbox',
            '#title' => t('Completed'),
            '#description' => t('Check if user has provided all the required files and pdfs.'),
        );
    } //$proposal_data->approval_status == 1
    if ($proposal_data->approval_status == 2) {
        $form['message'] = array(
            '#type' => 'item',
            '#markup' => $proposal_data->message,
            '#title' => t('Reason for disapproval'),
        );
    } //$proposal_data->approval_status == 2
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
    );
    $form['cancel'] = array(
        '#type' => 'markup',
        '#markup' => l(t('Cancel'), 'case-study-upgradation/manage-proposal/all'),
    );
    return $form;
}
function upgradation_proposal_status_form_submit($form, &$form_state)
{
    global $user;
    /* get current proposal */
    $proposal_id = (int) arg(3);
    //$proposal_q = db_query("SELECT * FROM {case_study_proposal} WHERE id = %d", $proposal_id);
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
            drupal_goto('case-study-upgradation/manage-proposal');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-upgradation/manage-proposal');
        return;
    }
    /* set the book status to completed */
    if ($form_state['values']['completed'] == 1) {
        $up_query = "UPDATE csu_proposal SET approval_status = :approval_status , actual_completion_date = :expected_completion_date WHERE id = :proposal_id";
        $args = array(
            ":approval_status" => '3',
            ":proposal_id" => $proposal_id,
            ":expected_completion_date" => time(),
        );
        $result = db_query($up_query, $args);
        CreateReadmeFileUpgradedCaseStudyProject($proposal_id);
        if (!$result) {
            drupal_set_message('Error in update status', 'error');
            return;
        } //!$result
        /* sending email */
        $user_data = user_load($proposal_data->uid);
        $email_to = $user_data->mail;
        $from = variable_get('upgraded_case_study_from_email', '');
        $bcc = $user->mail . ', ' . variable_get('upgraded_case_study_emails', '');
        $cc = variable_get('upgraded_case_study_cc_emails', '');
        $params['case_study_proposal_completed']['proposal_id'] = $proposal_id;
        $params['case_study_proposal_completed']['user_id'] = $proposal_data->uid;
        $params['case_study_proposal_completed']['headers'] = array(
            'From' => $from,
            'MIME-Version' => '1.0',
            'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
            'Content-Transfer-Encoding' => '8Bit',
            'X-Mailer' => 'Drupal',
            'Cc' => $cc,
            'Bcc' => $bcc,
        );
        if (!drupal_mail('case_study', 'case_study_proposal_completed', $email_to, language_default(), $params, $from, true)) {
            drupal_set_message('Error sending email message.', 'error');
        }

        drupal_set_message('Congratulations! CFD Case Study proposal has been marked as completed. User has been notified of the completion.', 'status');
    }
    drupal_goto('case-study-upgradation/manage-proposal');
    return;

}


/******************************************************************************/
/**************************** PROPOSAL EDIT FORM ******************************/
/******************************************************************************/
function upgradation_proposal_edit_form($form, &$form_state)
{
    global $user;
    /* get current proposal */
    $proposal_id = (int) arg(3);
    //$proposal_q = db_query("SELECT * FROM {case_study_proposal} WHERE id = %d", $proposal_id);
    $query = db_select('csu_proposal');
    $query->fields('csu_proposal');
    $query->condition('id', $proposal_id);
    $proposal_q = $query->execute();
    $proposal_data = $proposal_q->fetchObject();
    /*if ($proposal_q) {
        if ($proposal_data = $proposal_q->fetchObject()) {
            /* everything ok 
        } //$proposal_data = $proposal_q->fetchObject()
        else {
            drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
            drupal_goto('case-study-project/manage-proposal');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-project/manage-proposal');
        return;
    }*/
    $user_data = user_load($proposal_data->uid);
    $query = db_select('csu_version', 'v')
    ->fields('v', array('id', 'case_study_version'));
    $query->innerJoin('case_study_proposal', 'p', 'p.version_id = v.id');
    $query->condition('v.id', $proposal_data->version_id);
    $version_data = $query->execute()->fetchObject();
    if(!$version_data){
        $version = 'NA';
    }
    else{
    $version = $version_data->case_study_version;
    }
    $form['name_title'] = array(
        '#type' => 'select',
        '#title' => t('Title'),
        '#options' => array(
            'Dr' => 'Dr',
            'Prof' => 'Prof',
            'Mr' => 'Mr',
            'Ms' => 'Ms',
        ),
        '#required' => true,
        '#default_value' => $proposal_data->name_title,
    );
    $form['contributor_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of the Proposer'),
        '#size' => 30,
        '#maxlength' => 50,
        '#required' => true,
        '#default_value' => $proposal_data->contributor_name,
    );
    $form['student_email_id'] = array(
        '#type' => 'item',
        '#title' => t('Email'),
        '#markup' => $user_data->mail,
    );
    $form['university'] = array(
        '#type' => 'textfield',
        '#title' => t('University/Institute'),
        '#size' => 200,
        '#maxlength' => 200,
        '#required' => true,
        '#default_value' => $proposal_data->university,
    );
    $form['institute'] = array(
        '#type' => 'textfield',
        '#title' => t('Institute'),
        '#size' => 80,
        '#maxlength' => 200,
        '#required' => true,
        '#default_value' => $proposal_data->institute,
    );
    $form['how_did_you_know_about_project'] = array(
        '#type' => 'textfield',
        '#title' => t('How did you come to know about the Case Study Project?'),
        '#default_value' => $proposal_data->how_did_you_know_about_project,
        // '#required' => true,
    );
    $form['faculty_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name of the Faculty'),
        '#size' => 50,
        '#maxlength' => 50,
        '#validated' => true,
        '#default_value' => $proposal_data->faculty_name,
    );
    $form['faculty_department'] = array(
        '#type' => 'textfield',
        '#title' => t('Department of the Faculty'),
        '#size' => 50,
        '#maxlength' => 50,
        '#validated' => true,
        '#default_value' => $proposal_data->faculty_department,
    );
    $form['faculty_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email id of the Faculty'),
        '#size' => 255,
        '#maxlength' => 255,
        '#validated' => true,
        '#default_value' => $proposal_data->faculty_email,
    );
    $form['country'] = array(
        '#type' => 'select',
        '#title' => t('Country'),
        '#options' => array(
            'India' => 'India',
            'Others' => 'Others',
        ),
        '#default_value' => $proposal_data->country,
        '#required' => true,
        '#tree' => true,
        '#validated' => true,
    );
    $form['other_country'] = array(
        '#type' => 'textfield',
        '#title' => t('Other than India'),
        '#size' => 100,
        '#default_value' => $proposal_data->country,
        '#attributes' => array(
            'placeholder' => t('Enter your country name'),
        ),
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others',
                ),
            ),
        ),
    );
    $form['other_state'] = array(
        '#type' => 'textfield',
        '#title' => t('State other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your state/region name'),
        ),
        '#default_value' => $proposal_data->state,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others',
                ),
            ),
        ),
    );
    $form['other_city'] = array(
        '#type' => 'textfield',
        '#title' => t('City other than India'),
        '#size' => 100,
        '#attributes' => array(
            'placeholder' => t('Enter your city name'),
        ),
        '#default_value' => $proposal_data->city,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'Others',
                ),
            ),
        ),
    );
    $form['all_state'] = array(
        '#type' => 'select',
        '#title' => t('State'),
        '#options' => _df_list_of_states(),
        '#default_value' => $proposal_data->state,
        '#validated' => true,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India',
                ),
            ),
        ),
    );
    $form['city'] = array(
        '#type' => 'select',
        '#title' => t('City'),
        '#options' => _df_list_of_cities(),
        '#default_value' => $proposal_data->city,
        '#states' => array(
            'visible' => array(
                ':input[name="country"]' => array(
                    'value' => 'India',
                ),
            ),
        ),
    );
    $form['pincode'] = array(
        '#type' => 'textfield',
        '#title' => t('Pincode'),
        '#size' => 30,
        '#maxlength' => 6,
        '#default_value' => $proposal_data->pincode,
        '#attributes' => array(
            'placeholder' => 'Insert pincode of your city/ village....',
        ),
    );
    $list_case_study =  _csu_list_of_case_studies();
    $form['project_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Title of the Case Study Project'),
        '#disabled' => true,
        // '#options' => $list_case_study,
        '#required' => true,
        '#default_value' => $proposal_data->project_title,
    );
    // $version_options = ;proposal_id);
    // var_dump($proposal_data->project_title);die;
    
    $form['version'] = array(
        '#type' => 'select',
        '#title' => t('Version used'),
        '#options' => _csu_list_of_versions(),
        '#default_value' => $version_data->id,
        '#ajax' => array(
            // 'event'=>'change',
            'callback' =>'ajax_solver_callback',
            // 'wrapper' => 'dropdown-project-titles',
        ),
    );
//    solver used option function 
    $simulation_type_options = _csu_list_of_simulation_types();
    $form['simulation_type'] = array(
        '#type' => 'select',
        '#title' => t('Simulation Type used'),
        '#options' => $simulation_type_options,
        '#default_value' => $proposal_data->simulation_type_id,
        '#disabled' => true,
    );
// var_dump($proposal_data->solver_used);die;
  
    $form['solver_used'] = array(
		'#type' => 'select',
		'#title' => t('Select the Solver to be used'),
		'#options' => _csu_list_of_solvers($version_data->id),
		'#prefix' => '<div id="ajax-solver-replace">',
		'#suffix' => '</div>',
		'#default_value' => $proposal_data->solver_used,
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
    
 
    $form['date_of_proposal'] = array(
        '#type' => 'textfield',
        '#title' => t('Date of Proposal'),
        '#default_value' => date('d/m/Y', $proposal_data->creation_date),
        '#disabled' => true,
    );
    $form['delete_proposal'] = array(
        '#type' => 'checkbox',
        '#title' => t('Delete Proposal'),
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
    );
    $form['cancel'] = array(
        '#type' => 'item',
        '#markup' => l(t('Cancel'), 'case-study-upgradation/manage-proposal'),
    );
    return $form;
}

function ajax_solver_callback($form, &$form_state) {
    $version_name = $form_state['values']['version'];
    // var_dump($version_name);die;
//     $query = db_query("SELECT s.*, v.* FROM csu_solvers s
//     INNER JOIN csu_version v ON s.version_id = v.id
//     WHERE v.case_study_version = :version_name", array(":version_name" => $version_name));
// $solver_data = $query->execute()->fetchObject();
// $solvers = $solver_data->solver_name;

	// $version_name = isset($form_state['values']['version']) ? $form_state['values']['version'] : NULL;
    $form['solver_used']['#options'] = _csu_list_of_solvers($version_name);
	// $form['solver_used']['#required'] = TRUE;
	$commands[] = ajax_command_replace('#ajax-solver-replace', drupal_render($form['solver_used']));
	$commands[] = ajax_command_html('#ajax-solver-text-replace', '' );
	// $commands[] = ajax_command_replace('#ajax-solver-text-replace', drupal_render($form['solver_used_text']));
    // $solvers1 = $form_state['values']['solver_used'];
    // if ($solvers1 == 'Other') {
	// 	// $form['solver_used_text']['#required'] = FALSE;
	// 	$commands[] = ajax_command_replace('#ajax-solver-text-replace', drupal_render($form['solver_used_text']));
	// }
			
		// }
    return array(
        '#type' => 'ajax',
        '#commands' => $commands       
    );
    // 
}
function upgradation_proposal_edit_form_validate($form, $form_state){
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

}
function upgradation_proposal_edit_form_submit($form, &$form_state)
{
    global $user;
    /* get current proposal */
    $proposal_id = (int) arg(3);
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
            drupal_goto('case-study-upgradation/manage-proposal');
            return;
        }
    } //$proposal_q
    else {
        drupal_set_message(t('Invalid proposal selected. Please try again.'), 'error');
        drupal_goto('case-study-upgradation/manage-proposal');
        return;
    }
    /* delete proposal */
    if ($form_state['values']['delete_proposal'] == 1) {
        /* sending email */
        $user_data = user_load($proposal_data->uid);
        $email_to = $user_data->mail;
        $from = variable_get('upgraded_case_study_from_email', '');
        $bcc = variable_get('upgraded_case_study_emails', '');
        $cc = variable_get('upgraded_case_study_cc_emails', '');
        $params['case_study_proposal_deleted']['proposal_id'] = $proposal_id;
        $params['case_study_proposal_deleted']['user_id'] = $proposal_data->uid;
        $params['case_study_proposal_deleted']['headers'] = array(
            'From' => $from,
            'MIME-Version' => '1.0',
            'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
            'Content-Transfer-Encoding' => '8Bit',
            'X-Mailer' => 'Drupal',
            'Cc' => $cc,
            'Bcc' => $bcc,
        );
        if (!drupal_mail('case_study', 'case_study_proposal_deleted', $email_to, user_preferred_language($user), $params, $from, true)) {
            drupal_set_message('Error sending email message.', 'error');
        }

        drupal_set_message(t('Case Study proposal has been deleted.'), 'status');
        if (csu_rrmdir_project($proposal_id) == true) {
            $query = db_delete('csu_proposal');
            $query->condition('id', $proposal_id);
            $num_deleted = $query->execute();
            drupal_set_message(t('Proposal Deleted'), 'status');
            drupal_goto('case-study-upgradation/manage-proposal');
            return;
        } //rrmdir_project($proposal_id) == TRUE
    } //$form_state['values']['delete_proposal'] == 1
    /* update proposal */
    $v = $form_state['values'];
    $project_title = $form_state['values']['cfd_case_study_name_dropdown'];
    $proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
    $university = $v['university'];
    $directory_names = _csu_dir_name($project_title, $proposar_name);
    if (CSU_RenameDir($proposal_id, $directory_names)) {
        $directory_name = $directory_names;
    } //LM_RenameDir($proposal_id, $directory_names)
    else {
        return;
    }
    $simulation_id = $v['simulation_type'];
    if( $v['solver_used'] == 'Other'){
        $solver = $v['solver_used_text'];
    }
    else{
        $solver = $v['solver_used'];
    }
   
    $query = "UPDATE csu_proposal SET
				name_title=:name_title,
				contributor_name=:contributor_name,
				university=:university,
				institute=:institute,
				how_did_you_know_about_project = :how_did_you_know_about_project,
				faculty_name = :faculty_name,
				faculty_department = :faculty_department,
				faculty_email = :faculty_email,
				city=:city,
				pincode=:pincode,
				state=:state,
				project_title=:project_title,
                version_id=:version_id,
                simulation_type_id=:simulation_type_id,
				solver_used=:solver_used,
				directory_name=:directory_name
				WHERE id=:proposal_id";
    $args = array(
        ':name_title' => $v['name_title'],
        ':contributor_name' => $v['contributor_name'],
        ':university' => $v['university'],
        ":institute" => $v['institute'],
        ":how_did_you_know_about_project" => $v['how_did_you_know_about_project'],
        ":faculty_name" => $v['faculty_name'],
        ":faculty_department" => $v['faculty_department'],
        ":faculty_email" => $v['faculty_email'],
        ':city' => $v['city'],
        ':pincode' => $v['pincode'],
        ':state' => $v['all_state'],
        ':project_title' => $project_title,
        ":version_id" => $v['version'],
		":simulation_type_id" => $v['simulation_type'],
		":solver_used" => $solver,
        ':directory_name' => $directory_name,
        ':proposal_id' => $proposal_id,
    );
    $result = db_query($query, $args);
    drupal_set_message(t('Proposal Updated'), 'status');
}
