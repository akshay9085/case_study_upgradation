<?php
/*
Approval Status :
0 - Pending
1 - Approved
2 - Dis-Approved
3 - Completed
Solution Status :
0 - Pending
1 - Approved
2 - Dis-Approved
Solution Display :
0 - No
1 - Yes
is_completed
0 - in progress
1 - Completed
Tables :
case_study_solution : approval_status
0 - Pending
1 - Approved
2 - Disapproved (delete it)
*/
// csu(case study upgradation)
function csu_proposal_form($form, &$form_state, $no_js_use = FALSE)
{
	global $user;
	/************************ start approve book details ************************/
	if ($user->uid == 0)
	{
		$msg = drupal_set_message(t('It is mandatory to ' . l('login', 'user') . ' on this website to access the case study proposal form. If you are new user please create a new account first.'), 'error');
		drupal_goto('user');
		return $msg;
	} //$user->uid == 0
	$query = db_select('csu_proposal');
	$query->fields('csu_proposal');
	$query->condition('uid', $user->uid);
	$query->orderBy('id', 'DESC');
	$query->range(0, 1);
	$proposal_q = $query->execute();
	$proposal_data = $proposal_q->fetchObject();
	if ($proposal_data)
	{
		if ($proposal_data->approval_status == 0 || $proposal_data->approval_status == 1)
		{
			drupal_set_message(t('We have already received your proposal.'), 'status');
			drupal_goto('');
			return;
		} //$proposal_data->approval_status == 0 || $proposal_data->approval_status == 1
	} //$proposal_data
	$form['#attributes'] = array(
		'enctype' => "multipart/form-data"
	);

	$form['name_title'] = array(
		'#type' => 'select',
		'#title' => t('Title'),
		'#options' => array(
			'Dr' => 'Dr',
			'Prof' => 'Prof',
			'Mr' => 'Mr',
			'Ms' => 'Ms'
		),
		'#required' => TRUE
	);
	$form['contributor_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the contributor'),
		'#size' => 250,
		'#attributes' => array(
			'placeholder' => t('Enter your full name.....')
		),
		'#maxlength' => 250,
		'#required' => TRUE
	);
	$form['contributor_email_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Email'),
		'#size' => 30,
		'#value' => $user->mail,
		'#disabled' => TRUE
	);
	$form['contributor_contact_no'] = array(
		'#type' => 'textfield',
		'#title' => t('Contact No.'),
		'#size' => 10,
		'#attributes' => array(
			'placeholder' => t('Enter your contact number')
		),
		'#maxlength' => 250
	);
	$form['university'] = array(
		'#type' => 'textfield',
		'#title' => t('University'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your university.... '
		)
	);
	$form['institute'] = array(
		'#type' => 'textfield',
		'#title' => t('Institute'),
		'#size' => 80,
		'#maxlength' => 200,
		'#required' => TRUE,
		'#attributes' => array(
			'placeholder' => 'Insert full name of your institute.... '
		)
	);
	// $form['how_did_you_know_about_project'] = array(
	// 	'#type' => 'select',
	// 	'#title' => t('How did you come to know about the Case Study Project?'),
	// 	'#options' =>  array(
	// 		'Poster' => 'Poster',
	// 		'Website' => 'Website',
	// 		'Email' => 'Email',
	// 		'Others' => 'Others'
	// 	),
	// 	'#required' => TRUE
	// );
	// $form['others_how_did_you_know_about_project'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t('If ‘Other’, please specify'),
	// 	'#maxlength' => 50,
	// 	'#description' => t('<span style="color:red">Maximum character limit is 50</span>'),
	// 	'#states' => array(
	// 		'visible' => array(
	// 			':input[name="how_did_you_know_about_project"]' => array(
	// 				'value' => 'Others'
	// 			)
	// 		)
	// 	)		
	// );
	$form['faculty_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Name of the Faculty Member of your Institution, if any, who helped you with this Case Study Project'),
		'#size' => 50,
		'#maxlength' => 50,
		'#validated' => TRUE,
		'#description' => t('<span style="color:red">Maximum character limit is 50</span>')
	);
	$form['faculty_department'] = array(
		'#type' => 'textfield',
		'#title' => t('Department of the Faculty Member of your Institution, if any, who helped you with this Case Study Project'),
		'#size' => 50,
		'#maxlength' => 50,
		'#validated' => TRUE,
		'#description' => t('<span style="color:red">Maximum character limit is 50</span>')
	);
	$form['faculty_email'] = array(
		'#type' => 'textfield',
		'#title' => t('Email id of the Faculty Member of your Institution, if any, who helped you with this Case Study Project'),
		'#size' => 255,
		'#maxlength' => 255,
		'#validated' => TRUE,
		'#description' => t('<span style="color:red">Maximum character limit is 255</span>')
	);
	$form['country'] = array(
		'#type' => 'select',
		'#title' => t('Country'),
		'#options' => array(
			'India' => 'India',
			'Others' => 'Others'
		),
		'#required' => TRUE,
		'#tree' => TRUE,
		'#validated' => TRUE
	);
	$form['other_country'] = array(
		'#type' => 'textfield',
		'#title' => t('Other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your country name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_state'] = array(
		'#type' => 'textfield',
		'#title' => t('State other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your state/region name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['other_city'] = array(
		'#type' => 'textfield',
		'#title' => t('City other than India'),
		'#size' => 100,
		'#attributes' => array(
			'placeholder' => t('Enter your city name')
		),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'Others'
				)
			)
		)
	);
	$form['all_state'] = array(
		'#type' => 'select',
		'#title' => t('State'),
		'#options' => _df_list_of_states(),
		'#validated' => TRUE,
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['city'] = array(
		'#type' => 'select',
		'#title' => t('City'),
		'#options' => _df_list_of_cities(),
		'#states' => array(
			'visible' => array(
				':input[name="country"]' => array(
					'value' => 'India'
				)
			)
		)
	);
	$form['pincode'] = array(
		'#type' => 'textfield',
		'#title' => t('Pincode'),
		'#size' => 6,
		'#maxlength' => 6
	);
	/***************************************************************************/
	$form['hr'] = array(
		'#type' => 'item',
		'#markup' => '<hr>'
	);
	$version_options = _csu_list_of_versions();
	$form['version'] = array(
		'#type' => 'select',
		'#title' => t('Select the version to be used'),
		'#options' =>  	$version_options,
		
		'#required' => TRUE,
        '#ajax' => array(
            // 'event'=>'change',
            'callback' =>'ajax_solver_callback',
            // 'wrapper' => 'dropdown-project-titles',
        ),	
	);
	
	$list_case_study =  _csu_list_of_case_studies();
	$form['cfd_case_study_name_dropdown'] = array(
            '#type' => 'select',
            '#title' => t('Project Title (Completed case studies)'),
            '#required' => TRUE,
            '#options' => _csu_list_of_case_studies(),
			'#chosen' => FALSE,
           	// '#validated' => TRUE,
            '#states' => array(
                'invisible' => array(
                    ':input[name="version"]' => array(
                        'value' => ''
                    )
                )   
            ),
			'#ajax' => array(
						// 'event'=>'change',
						'callback' =>'ajax_simulations_callback',
						// 'wrapper' => 'dropdown-project-titles',
			),	
            // '#prefix' => '<div id = "dropdown-project-titles">',
            // '#suffix' => '</div>',
            
	);
	$case_study_name = isset($form_state['values']['cfd_case_study_name_dropdown'])? $form_state['values']['cfd_case_study_name_dropdown'] : NULL ;
	$simulation_type_options = _csu_list_of_simulation_types();
	$form['simulation_type'] = array(
		'#type' => 'textfield',
		'#prefix' => '<div id="ajax-simulations-replace">',
		'#suffix' => '</div>',
		'#title' => t('Simulation Type used'),

		'#default_value' => _csu_list_of_simulation_types($case_study_name),
		// '#required' => TRUE,
		'#disabled' => true,
		'#states' => array(
			'invisible' => array(
				':input[name="cfd_case_study_name_dropdown"]' => array(
					'value' => ''
				)
			)   
		)
		
	);
		// var_dump( _csu_list_of_simulation_types('Water flow simulation in an elbow draft tube'));die;
	$form['solver_used'] = array(
		'#type' => 'textfield',
		'#title' => t('Solver used'),
		'#prefix' => '<div id="ajax-solver-replace">',
		'#suffix' => '</div>',
		'#default_value' => _csu_old_list_of_solver($case_study_name),
		// '#required' => TRUE,
		'#disabled' => true,
		'#states' => array(
			'invisible' => array(
				':input[name="cfd_case_study_name_dropdown"]' => array(
					'value' => ''
				)
			)   
		),
		// '#prefix' => '<div id="ajax-simulations-replace">',
		// '#suffix' => '</div>'
	);
	// var_dump(_csu_old_list_of_solver('CFD analysis of a centrifugal pump'));die;
	// var_dump($simulation_type_options);die;
    
	// $version_id = isset($form_state['values']['version']) ? $form_state['values']['version'] : NULL ;
    // // var_dump($version_id );die;
	// // if(!empty($version_id)){_csu_list_of_solvers($version_id)
	// $solver_options = _csu_list_of_solvers($version_id);
	// $form['solver_used'] = array(
	// 	'#type' => 'select',
	// 	'#title' => t('Select the Solver to be used'),
	// 	'#options' =>_csu_list_of_solvers($version_id) ,
	// 	'#prefix' => '<div id="ajax-solver-replace">',
	// 	'#suffix' => '</div>',
	// 	// '#default_value' = 
	// 	// '#ajax' => array(
    //     //     // 'event'=>'change',
    //     //     'callback' =>'ajax_other_solver_callback',
    //     //     // 'wrapper' => 'dropdown-project-titles',
    //     // ),
	// 	'#required' => TRUE,
	// 	'#states' => array(
	// 		'invisible' => array(
	// 			':input[name="version"]' => array(
	// 				'value' => ''
	// 			)
	// 		)
	// 	),

	// );
	// // $solver_id = isset($form_state['values']['solver_used']) ? $form_state['values']['solver_used'] : _csu_list_of_solvers($version_id);
	// // // }
	// // $solvers = $form_state['values']['solver_used'];
	// // //else if ($simulation_id == 19){
	// // 	if($solvers == 'Other'){
	// $form['solver_used_text'] = array(
	// 	'#type' => 'textfield',
	// 	'#title' => t("Enter the Solver to be used <span style='color:red;'>*</span>"),
	// 	'#size' => 100,
	// 	'#description' => t('Maximum character limit is 50'),
	// 	// '#required' => TRUE,
	// 	'#prefix' => '<div id="ajax-solver-text-replace">',
	// 	'#suffix' => '</div>',
	// 	// '#required' => TRUE,
	// 	'#states' => array(
    //             'visible' => array(
    //                 ':input[name="solver_used"]' => array(
    //                     'value' => 'Other'
    //                 )
    //             )
    //         ),     
	// );
    // }
	$form['date_of_proposal'] = array(
		'#type' => 'date_popup',
		'#title' => t('Date of Proposal'),
		'#default_value' => date("Y-m-d H:i:s"),
		'#date_format' => 'd M Y',
		'#disabled' => TRUE,
		'#date_label_position' => '',
	);
	$form['expected_date_of_completion'] = array(
		'#type' => 'date_popup',
		'#title' => t('Expected Date of Completion'),
		'#date_label_position' => '',
		'#description' => '',
		'#default_value' => '',
		'#date_format' => 'd-M-Y',
		//'#date_increment' => 0,
		//'#minDate' => '+0',
		'#date_year_range' => '0 : +1',
		'#required' => TRUE,
		'#datepicker_options' => array(
			'maxDate' => '+45D', // not more than current date
			'minDate' => '+1' // not more than given date
		),
	);
	
	$form['term_condition'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Terms And Conditions'),
		'#options' => array(
			'status' => t('<a href="/case-study-project/term-and-conditions" target="_blank">I agree to the Terms and Conditions</a>')
		),
		'#required' => TRUE
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	return $form;
}
function ajax_simulations_callback($form, &$form_state) {
	$commands[] = ajax_command_replace('#ajax-simulations-replace', drupal_render($form['simulation_type']));
	$commands[] = ajax_command_replace('#ajax-solver-replace', drupal_render($form['solver_used']));
	return array(
        '#type' => 'ajax',
        '#commands' => $commands       
    );
}
// function ajax_solver_callback($form, &$form_state) {

// 	$version_id = isset($form_state['values']['version']) ? $form_state['values']['version'] : NULL;
//     // $form['solver_used']['#options'] = _csu_list_of_solvers($version_id);
// 	$form['solver_used']['#required'] = TRUE;
// 	$commands[] = ajax_command_replace('#ajax-solver-replace', drupal_render($form['solver_used']));
// 	// $commands[] = ajax_command_html('#ajax-solver-text-replace', '' );
// 	// $commands[] = ajax_command_replace('#ajax-solver-text-replace', drupal_render($form['solver_used_text']));
//     $solvers1 = $form_state['values']['solver_used'];
//     // if ($solvers1 == 'Other') {
// 	// 	// $form['solver_used_text']['#required'] = FALSE;
// 	// 	$commands[] = ajax_command_replace('#ajax-solver-text-replace', drupal_render($form['solver_used_text']));
// 	// }
			
// 		// }
//     return array(
//         '#type' => 'ajax',
//         '#commands' => $commands       
//     );
// }



function csu_proposal_form_validate($form, &$form_state)
{
	//var_dump($form_state['values']['solver_used']);die;
	// if($form_state['values']['cfd_project_title_check'] == 1)
	// {
	// 	$project_title = $form_state['values']['cfd_case_study_name_dropdown'];
	// }
	// else{

	// 	$project_title = $form_state['values']['project_title'];
	// }
	if ($form_state['values']['term_condition'] == '1')
	{
		form_set_error('term_condition', t('Please check the terms and conditions'));
		// $form_state['values']['country'] = $form_state['values']['other_country'];
	} //$form_state['values']['term_condition'] == '1'
	if ($form_state['values']['country'] == 'Others')
	{
		if ($form_state['values']['other_country'] == '')
		{
			form_set_error('other_country', t('Enter country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_country'] == ''
		else
		{
			$form_state['values']['country'] = $form_state['values']['other_country'];
		}
		if ($form_state['values']['other_state'] == '')
		{
			form_set_error('other_state', t('Enter state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_state'] == ''
		else
		{
			$form_state['values']['all_state'] = $form_state['values']['other_state'];
		}
		if ($form_state['values']['other_city'] == '')
		{
			form_set_error('other_city', t('Enter city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_city'] == ''
		else
		{
			$form_state['values']['city'] = $form_state['values']['other_city'];
		}
	} //$form_state['values']['country'] == 'Others'
	else
	{
		if ($form_state['values']['country'] == '')
		{
			form_set_error('country', t('Select country name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['country'] == ''
		if ($form_state['values']['all_state'] == '')
		{
			form_set_error('all_state', t('Select state name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['all_state'] == ''
		if ($form_state['values']['city'] == '')
		{
			form_set_error('city', t('Select city name'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['city'] == ''
	}
	//Validation for project title
	// $form_state['values']['project_title'] = trim($form_state['values']['project_title']);
	// if ($form_state['values']['project_title'] != '')
	// {
	// 	if (strlen($form_state['values']['project_title']) > 250)
	// 	{
	// 		form_set_error('project_title', t('Maximum charater limit is 250 charaters only, please check the length of the project title'));
	// 	} //strlen($form_state['values']['project_title']) > 250
	// 	else if (strlen($form_state['values']['project_title']) < 10)
	// 	{
	// 		form_set_error('project_title', t('Minimum charater limit is 10 charaters, please check the length of the project title'));
	// 	} //strlen($form_state['values']['project_title']) < 10
	// } //$form_state['values']['project_title'] != ''
	// /*else
	// {
	// 	form_set_error('project_title', t('Project title shoud not be empty'));
	// }*/
	
	

	// if(strtotime(date($form_state['values']['expected_date_of_completion'])) < time())
	// {
	// 	form_set_error('expected_date_of_completion', t('Completion date should not be earlier than proposal date'));
	// }

	if ($form_state['values']['how_did_you_know_about_project'] == 'Others')
	{
		if ($form_state['values']['others_how_did_you_know_about_project'] == '')
		{
			form_set_error('others_how_did_you_know_about_project', t('Please enter how did you know about the project'));
			// $form_state['values']['country'] = $form_state['values']['other_country'];
		} //$form_state['values']['other_country'] == ''
		else
		{
			$form_state['values']['how_did_you_know_about_project'] = $form_state['values']['others_how_did_you_know_about_project'];
		}
	}
	/*if ($form_state['values']['faculty_name'] != '' || $form_state['values']['faculty_name'] != "NULL") {
		if($form_state['values']['faculty_email'] == '' || $form_state['values']['faculty_email'] == "NULL")
		{
			form_set_error('faculty_email', t('Please enter the email id of your faculty'));
		}
		if($form_state['values']['faculty_department'] == '' || $form_state['values']['faculty_department'] == 'NULL'){
			form_set_error('faculty_department', t('Please enter the Department of your faculty'));
		}
	}*/
	
	return $form_state;
}
function csu_proposal_form_submit($form, &$form_state)
{
	global $user;
	// $root_path = case_study_path();
	if (!$user->uid)
	{
		drupal_set_message('It is mandatory to login on this website to access the proposal form', 'error');
		return;
	}
	// if($form_state['values']['cfd_project_title_check'] == 1)
	// {
		$project_title = $form_state['values']['cfd_case_study_name_dropdown'];
	// }
	// else{

	// 	$project_title = $form_state['values']['project_title'];
	// }
	if ($form_state['values']['how_did_you_know_about_project'] == 'Others')
	{
			$how_did_you_know_about_project = $form_state['values']['others_how_did_you_know_about_project'];
	}
	else {
		$how_did_you_know_about_project = $form_state['values']['how_did_you_know_about_project'];
	}
	/* inserting the user proposal */
	$v = $form_state["values"];
	// $project_title = trim($project_title);
	$proposar_name = $v['name_title'] . ' ' . $v['contributor_name'];
	$university = $v['university'];
	$directory_name = _csu_dir_name($project_title, $proposar_name);
	$simulation_id = $v['simulation_type'];
	// if( $v['solver_used'] === 'Other'){
	// 	$solver = $v['solver_used_text'];
	// }
	// else{
		$solver = $v['solver_used'];
	// }
	$result = "INSERT INTO {csu_proposal} 
    (
    uid, 
    approver_uid,
    name_title, 
    contributor_name,
    contact_no,
    university,
    institute,
    faculty_name,
    faculty_department,
    faculty_email,
    city, 
    pincode, 
    state, 
    country,
    project_title, 
    version_id,
    simulation_type_id,
    solver_used,
    directory_name,
    approval_status,
    is_completed, 
    dissapproval_reason,
    creation_date, 
    approval_date,
	expected_date_of_completion
    ) VALUES
    (
    :uid, 
    :approver_uid, 
    :name_title, 
    :contributor_name, 
    :contact_no,
    :university, 
    :institute,
    :faculty_name,
    :faculty_department,
    :faculty_email,
    :city, 
    :pincode, 
    :state,  
    :country,
    :project_title, 
    :version_id,
    :simulation_type_id,
    :solver_used,
	:directory_name,
    :approval_status,
    :is_completed, 
    :dissapproval_reason,
    :creation_date, 
	:expected_date_of_completion,
    :approval_date
    )";
	$args = array(
		":uid" => $user->uid,
		":approver_uid" => 0,
		":name_title" => $v['name_title'],
		":contributor_name" => _df_sentence_case(trim($v['contributor_name'])),
		":contact_no" => $v['contributor_contact_no'],
		":university" => $v['university'],
		":institute" => _df_sentence_case($v['institute']),
		":faculty_name" => $v['faculty_name'],
		":faculty_department" => $v['faculty_department'],
		":faculty_email" => $v['faculty_email'],
		":city" => $v['city'],
		":pincode" => $v['pincode'],
		":state" => $v['all_state'],
		":country" => $v['country'],
		":project_title" => $project_title,
		":version_id" => $v['version'],
		":simulation_type_id" => $simulation_id,
		":solver_used" => $solver,
		":directory_name" => $directory_name,
		":approval_status" => 1,
		":is_completed" => 0,
		":dissapproval_reason" => "NULL",
		":creation_date" => time(),
		":expected_date_of_completion" => strtotime(date($v['expected_date_of_completion'])),
		":approval_date" => 0
		);
	$result1 = db_query($result, $args, array(
		'return' => Database::RETURN_INSERT_ID
	));
	//var_dump($result1->id);die;
	$query_pro = db_select('csu_proposal');
	$query_pro->fields('csu_proposal');
//	$query_pro->condition('id', $proposal_data->id);
	$abstracts_pro = $query_pro->execute()->fetchObject();
//	$proposal_id = $abstracts_pro->id;
	// $dest_path = $directory_name . '/';
	// $dest_path1 = $root_path . $dest_path;
	// if (!is_dir($root_path . $dest_path))
	// 	mkdir($root_path . $dest_path);
	// /* uploading files */
	// foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
	// {
	// 	if ($file_name)
	// 	{
	// 		/* checking file type */
	// 		//$file_type = 'S';
	// 		if (file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
	// 		{
	// 			 drupal_set_message(t("Error uploading file. File !filename already exists.", array('!filename' => $_FILES['files']['name'][$file_form_name])), 'error');
	// 			//unlink($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]);
	// 		} //file_exists($root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
	// 		/* uploading file */
	// 		if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name]))
	// 		{
	// 			$query_pro = db_select('csu_proposal');
	// $query_pro->fields('csu_proposal');
	// //$query_pro->condition('id', $proposal_data->id);
	// $abstracts_pro = $query_pro->execute()->fetchObject();
	// //$proposal_id = $abstracts_pro->id;
	// //var_dump($proposal_id);die;
	// 			//$proposal_id = $result1->id;
	// 			$query_abstracts = "INSERT INTO {case_study_submitted_abstracts} (
	// proposal_id,
	// approver_uid,
	// abstract_approval_status,
	// abstract_upload_date,
	// abstract_approval_date,
	// is_submitted) VALUES (:proposal_id, :approver_uid, :abstract_approval_status,:abstract_upload_date, :abstract_approval_date, :is_submitted)";
	// 	$args = array(
	// 		":proposal_id" => $result1,
	// 		":approver_uid" => 0,
	// 		":abstract_approval_status" => 0,
	// 		":abstract_upload_date" => time(),
	// 		":abstract_approval_date" => 0,
	// 		":is_submitted" => 0
	// 	);
	// 	$submitted_abstract_id = db_query($query_abstracts, $args, array(
	// 		'return' => Database::RETURN_INSERT_ID
	// 	));
	// 		$query = "INSERT INTO {case_study_submitted_abstracts_file} (submitted_abstract_id, proposal_id, uid, approvar_uid, filename, filepath, filemime, filesize, filetype, timestamp)
    //       VALUES (:submitted_abstract_id, :proposal_id, :uid, :approvar_uid, :filename, :filepath, :filemime, :filesize, :filetype, :timestamp)";
	// 						$args = array(
	// 							":submitted_abstract_id" => $submitted_abstract_id,
	// 							":proposal_id" => $result1,
	// 							":uid" => $user->uid,
	// 							":approvar_uid" => 0,
	// 							":filename" => $_FILES['files']['name'][$file_form_name],
	// 							":filepath" => $_FILES['files']['name'][$file_form_name],
	// 							":filemime" => mime_content_type($root_path . $dest_path . $_FILES['files']['name'][$file_form_name]),
	// 							":filesize" => $_FILES['files']['size'][$file_form_name],
	// 							":filetype" => 'A',
	// 							":timestamp" => time()
	// 						);

	// 			/*$query = "UPDATE {case_study_proposal} SET abstract_file_path = :abstract_file_path WHERE id = :id";
	// 			$args = array(
	// 				":abstract_file_path" => $dest_path . $_FILES['files']['name'][$file_form_name],
	// 				":id" => $result1
	// 			);*/
				
	// 			$updateresult = db_query($query, $args);
	// 			//var_dump($args);die;

	// 			drupal_set_message($file_name . ' uploaded successfully.', 'status');
	// 		} //move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . $_FILES['files']['name'][$file_form_name])
	// 		else
	// 		{
	// 			drupal_set_message('Error uploading file : ' . $dest_path . '/' . $file_name, 'error');
	// 		}
	// 	} //$file_name
	// } //$_FILES['files']['name'] as $file_form_name => $file_name
	if (!$result1)
	{
		drupal_set_message(t('Error receiving your proposal. Please try again.'), 'error');
		return;
	} //!$proposal_id
	/* sending email */
	$email_to = $user->mail;
	$form = variable_get('case_study_from_email', '');
	$bcc = variable_get('case_study_emails', '');
	$cc = variable_get('case_study_cc_emails', '');
	$params['case_study_proposal_received']['result1'] = $result1;
	$params['case_study_proposal_received']['user_id'] = $user->uid;
	$params['case_study_proposal_received']['headers'] = array(
		'From' => $form,
		'MIME-Version' => '1.0',
		'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
		'Content-Transfer-Encoding' => '8Bit',
		'X-Mailer' => 'Drupal',
		'Cc' => $cc,
		'Bcc' => $bcc
	);
	if (!drupal_mail('case_study', 'case_study_proposal_received', $email_to, user_preferred_language($user), $params, $form, TRUE))
		drupal_set_message('Error sending email message.', 'error');
	drupal_set_message(t('We have received your case study proposal. Please submit your files.'), 'status');
	drupal_goto('case-study-upgradation/abstract-code');
}
// code upload url 

// function csu_path() {
//   return $_SERVER['DOCUMENT_ROOT'] . base_path() . 'cfd_uploads/case_study_upgradation_uploads/';
// }
