<?php

namespace Drupal\upgradation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Url;

class CsuProposalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'csu_proposal_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, $no_js_use = NULL) {
    $user = $this->currentUser();
    $latest_proposal = upgradation_load_latest_user_proposal($user->id());
    if ($latest_proposal && in_array((int) $latest_proposal->approval_status, [0, 1], TRUE)) {
      $this->messenger()->addStatus($this->t('We have already received your proposal.'));
      return $this->buildRedirectNotice(
        $this->t('You already have an active case study proposal.'),
        'upgradation.abstract',
        $this->t('Go to your submission page')
      );
    }

    $selected_case_study = (string) $form_state->getValue('cfd_case_study_name_dropdown', '');
    $details = $this->getCaseStudyDisplayDetails($selected_case_study);
    
/*****************Personal Information******************** */
    $form['name_title'] = [
      '#type' => 'select',
      '#title' => $this->t('Title'),
      '#options' => [
        'Dr' => 'Dr',
        'Prof' => 'Prof',
        'Mr' => 'Mr',
        'Ms' => 'Ms',
      ],
      '#required' => TRUE,
    ];

    $form['contributor_name'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Name of the contributor'),
      '#attributes' => [
        'placeholder' => $this->t('Enter your full name'),
      ],
      '#maxlength' => 250,
      '#required' => TRUE,
    ];

    $form['contributor_email_id'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Email'),
      '#value' => $user->getEmail(),
      '#disabled' => TRUE,
    ];

    $form['contributor_contact_no'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Contact No.'),
      '#attributes' => [
        'placeholder' => $this->t('Enter your contact number'),
      ],
      '#maxlength' => 250,
    ];

    $form['university'] = [
      '#type' => 'textfield',
      '#title' => $this->t('University'),
      '#maxlength' => 200,
      '#required' => TRUE,
      '#attributes' => [
        'placeholder' => $this->t('Insert the full name of your university'),
      ],
    ];

    $form['institute'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Institute'),
      '#maxlength' => 200,
      '#required' => TRUE,
      '#attributes' => [
        'placeholder' => $this->t('Insert the full name of your institute'),
      ],
    ];

    $form['faculty_name'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Name of the faculty member of your institution, if any, who helped you with this case study project'),
      '#maxlength' => 50,
      '#description' => $this->t('Maximum character limit is 50.'),
    ];

    $form['faculty_department'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Department of the faculty member of your institution, if any, who helped you with this case study project'),
      '#maxlength' => 50,
      '#description' => $this->t('Maximum character limit is 50.'),
    ];

    $form['faculty_email'] = [
      '#type' => 'email',
      '#title' => $this->t('Email id of the faculty member of your institution, if any, who helped you with this case study project'),
      '#maxlength' => 255,
      '#description' => $this->t('Maximum character limit is 255.'),
    ];

    $form['country'] = [
      '#type' => 'select',
      '#title' => $this->t('Country'),
      '#options' => [
        'India' => 'India',
        'Others' => 'Others',
      ],
      '#required' => TRUE,
    ];

    $form['other_country'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Other than India'),
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'Others'],
        ],
      ],
    ];

    $form['other_state'] = [
      '#type' => 'textfield',
      '#title' => $this->t('State other than India'),
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'Others'],
        ],
      ],
    ];

    $form['other_city'] = [
      '#type' => 'textfield',
      '#title' => $this->t('City other than India'),
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'Others'],
        ],
      ],
    ];

    $form['all_state'] = [
      '#type' => 'select',
      '#title' => $this->t('State'),
      '#options' => _df_list_of_states(),
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'India'],
        ],
      ],
    ];

    $form['city'] = [
      '#type' => 'select',
      '#title' => $this->t('City'),
      '#options' => _df_list_of_cities(),
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'India'],
        ],
      ],
    ];

    $form['pincode'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Pincode'),
      '#maxlength' => 6,
    ];
/*****************Personal Information End******************** */
    $form['hr'] = [
      '#type' => 'item',
      '#markup' => '<hr>',
    ];

    $form['case_study_selection'] = [
      '#type' => 'container',
      '#prefix' => '<div id="case-study-selection-wrapper">',
      '#suffix' => '</div>',
    ];

    $form['case_study_selection']['cfd_case_study_name_dropdown'] = [
      '#type' => 'select',
      '#title' => $this->t('Project title (completed case studies)'),
      '#options' => _csu_list_of_case_studies(),
      '#empty_option' => $this->t('- Select project title -'),
      '#default_value' => $selected_case_study ?: NULL,
      '#required' => TRUE,
      '#ajax' => [
        'callback' => '::updateCaseStudySelection',
        'wrapper' => 'case-study-selection-wrapper',
      ],
    ];

    if ($details) {
      $form['case_study_selection']['case_study_details'] = [
        '#type' => 'fieldset',
        '#title' => $this->t('earlier use'),
      ];

      $form['case_study_selection']['case_study_details']['earlier_version_used_display'] = [
        '#type' => 'item',
        '#title' => $this->t('Earlier version used'),
        '#markup' => $details['earlier_version_used'],
      ];

      $form['case_study_selection']['case_study_details']['simulation_type_display'] = [
        '#type' => 'item',
        '#title' => $this->t('Simulation type used'),
        '#markup' => $details['simulation_type'],
      ];

      $form['case_study_selection']['case_study_details']['solver_used_display'] = [
        '#type' => 'item',
        '#title' => $this->t('Solver used'),
        '#markup' => $details['solver_used'],
      ];

      $form['case_study_selection']['simulation_type'] = [
        '#type' => 'hidden',
        '#value' => $details['simulation_type'],
      ];

      $form['case_study_selection']['solver_used'] = [
        '#type' => 'hidden',
        '#value' => $details['solver_used'],
      ];

      $selected_version = $form_state->getValue('version');
      $solver_options = $selected_version ? _csu_list_of_solvers($selected_version) : [];
      $solver_options['Other'] = $this->t('Other');

      $form['case_study_selection']['version'] = [
        '#type' => 'select',
        '#title' => $this->t('Select the version to be used'),
        '#options' => _csu_list_of_versions(),
        '#empty_option' => $this->t('- Select version -'),
        '#required' => TRUE,
        '#default_value' => $selected_version,
        '#ajax' => [
          'callback' => '::updateCaseStudySelection',
          'wrapper' => 'case-study-selection-wrapper',
        ],
      ];

      

      $form['case_study_selection']['updated_solver_used'] = [
        '#type' => 'select',
        '#title' => $this->t('Updated solver to be used'),
        '#options' => $solver_options,
        '#empty_option' => $this->t('- Select solver -'),
        '#default_value' => $form_state->getValue('updated_solver_used'),
        '#required' => TRUE,
      ];

      $form['case_study_selection']['updated_solver_used_text'] = [
        '#type' => 'textfield',
        '#title' => $this->t('Enter the updated solver to be used'),
        '#maxlength' => 100,
        '#description' => $this->t('Maximum character limit is 100.'),
        '#states' => [
          'visible' => [
            ':input[name="updated_solver_used"]' => ['value' => 'Other'],
          ],
        ],
      ];
      $form['case_study_selection']['updated_simulation_type'] = [
        '#type' => 'select',
        '#title' => $this->t('Updated simulation type'),
        '#options' => _csu_list_of_simulation_types(),
        '#empty_option' => $this->t('- Select simulation type -'),
        // '#default_value' => $form_state->getValue('updated_simulation_type') ?: $details['simulation_type'],
        '#required' => TRUE,
      ];
    }

    $form['date_of_proposal'] = [
      '#type' => 'item',
      '#title' => $this->t('Date of Proposal'),
      '#markup' => date('d M Y'),
    ];
    $today = date('Y-m-d');
    $min_date = date('Y-m-d', strtotime('+1 day'));
    $max_date = date('Y-m-d', strtotime('+45 days'));
    $form['expected_date_of_completion'] = [
      '#type' => 'date',
      '#title' => $this->t('Expected Date of Completion'),
      '#required' => TRUE,
      '#default_value' => $today,
      '#attributes' => [
        'min' => $min_date,
        'max' => $max_date,
      ],
    ];

    $form['term_condition'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('I agree to the Terms and Conditions'),
      '#description' => $this->t('Please review the terms before submitting your proposal.'),
      '#required' => TRUE,
    ];

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Submit'),
    ];

    return $form;
  }

  /**
   * Ajax callback for the selected case study.
   */
  public function updateCaseStudySelection(array &$form, FormStateInterface $form_state) {
    return $form['case_study_selection'];
  }

  /**
   * Returns display details for a selected original case study.
   */
  private function getCaseStudyDisplayDetails($project_title) {
    if (!$project_title) {
      return [];
    }

    $case_study = \Drupal::database()->select('case_study_proposal', 'cp')
      ->fields('cp', ['version_id', 'simulation_type_id', 'solver_used'])
      ->condition('project_title', $project_title)
      ->range(0, 1)
      ->execute()
      ->fetchObject();

    if (!$case_study) {
      return [];
    }

    $earlier_version = upgradation_get_old_version_name($case_study->version_id) ?: upgradation_get_version_name($case_study->version_id) ?: '';
    $simulation_type = \Drupal::database()->select('case_study_simulation_type', 'st')
      ->fields('st', ['simulation_type'])
      ->condition('id', $case_study->simulation_type_id)
      ->range(0, 1)
      ->execute()
      ->fetchField();

    return [
      'earlier_version_used' => $earlier_version?: 'NA',
      'simulation_type' => $simulation_type ?: $case_study->simulation_type_id,
      'solver_used' => $case_study->solver_used ?: '',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    if (!$form_state->getValue('term_condition')) {
      $form_state->setErrorByName('term_condition', $this->t('Please agree to the terms and conditions.'));
    }

    if ($form_state->getValue('country') === 'Others') {
      if (!trim($form_state->getValue('other_country'))) {
        $form_state->setErrorByName('other_country', $this->t('Enter country name.'));
      }
      else {
        $form_state->setValue('country', trim($form_state->getValue('other_country')));
      }

      if (!trim($form_state->getValue('other_state'))) {
        $form_state->setErrorByName('other_state', $this->t('Enter state name.'));
      }
      else {
        $form_state->setValue('all_state', trim($form_state->getValue('other_state')));
      }

      if (!trim($form_state->getValue('other_city'))) {
        $form_state->setErrorByName('other_city', $this->t('Enter city name.'));
      }
      else {
        $form_state->setValue('city', trim($form_state->getValue('other_city')));
      }
    }
    else {
      if (!$form_state->getValue('all_state')) {
        $form_state->setErrorByName('all_state', $this->t('Select state name.'));
      }
      if (!$form_state->getValue('city')) {
        $form_state->setErrorByName('city', $this->t('Select city name.'));
      }
    }

    $expected_date = $form_state->getValue('expected_date_of_completion');
    if (!$expected_date || strtotime($expected_date) <= time()) {
      $form_state->setErrorByName('expected_date_of_completion', $this->t('Choose a future completion date.'));
    }

    if (!$form_state->getValue('cfd_case_study_name_dropdown')) {
      $form_state->setErrorByName('cfd_case_study_name_dropdown', $this->t('Select a case study project title.'));
    }

    if (!$form_state->getValue('version')) {
      $form_state->setErrorByName('version', $this->t('Select the version to be used.'));
    }

    if (!$form_state->getValue('updated_simulation_type')) {
      $form_state->setErrorByName('updated_simulation_type', $this->t('Select the updated simulation type.'));
    }

    $this->validateUpdatedSolverField($form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $user = $this->currentUser();
    $values = $form_state->getValues();
    $project_title = $values['cfd_case_study_name_dropdown'];
    $proposer_name = $values['name_title'] . ' ' . $values['contributor_name'];
    $directory_name = _csu_dir_name($project_title, $proposer_name);
    $solver = $values['updated_solver_used'] === 'Other' ? trim((string) $values['updated_solver_used_text']) : $values['updated_solver_used'];

    $proposal_id = \Drupal::database()->insert('csu_proposal')
      ->fields([
        'uid' => (int) $user->id(),
        'approver_uid' => 0,
        'name_title' => $values['name_title'],
        'contributor_name' => _df_sentence_case(trim($values['contributor_name'])),
        'contact_no' => $values['contributor_contact_no'],
        'university' => $values['university'],
        'institute' => _df_sentence_case($values['institute']),
        'faculty_name' => $values['faculty_name'],
        'faculty_department' => $values['faculty_department'],
        'faculty_email' => $values['faculty_email'],
        'city' => $values['city'],
        'pincode' => $values['pincode'],
        'state' => $values['all_state'],
        'country' => $values['country'],
        'project_title' => $project_title,
        'version_id' => $values['version'],
        'simulation_type_id' => $values['updated_simulation_type'],
        'solver_used' => $solver,
        'directory_name' => $directory_name,
        'approval_status' => 1,
        'is_completed' => 0,
        'dissapproval_reason' => 'NULL',
        'creation_date' => time(),
        'approval_date' => 0,
        'expected_date_of_completion' => strtotime($values['expected_date_of_completion']),
      ])
      ->execute();

    if (!$proposal_id) {
      $this->messenger()->addError($this->t('Error receiving your proposal. Please try again.'));
      return;
    }

    $mail = upgradation_mail_settings();
    $langcode = $user->getPreferredLangcode() ?: \Drupal::languageManager()->getDefaultLanguage()->getId();
    $params['case_study_proposal_received']['result1'] = $proposal_id;
    $params['case_study_proposal_received']['user_id'] = (int) $user->id();
    $params['case_study_proposal_received']['headers'] = [
      'From' => $mail['from'],
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $mail['cc'],
      'Bcc' => $mail['bcc'],
    ];

    if (!upgradation_send_mail('upgradation', 'case_study_proposal_received', $user->getEmail(), $langcode, $params, $mail['from'])) {
      $this->messenger()->addError($this->t('Error sending email message.'));
    }

    $this->messenger()->addStatus($this->t('We have received your case study proposal. Please submit your files.'));
    $form_state->setRedirect('upgradation.abstract');
  }

  /**
   * Validates updated solver-related fields.
   */
  private function validateUpdatedSolverField(FormStateInterface $form_state) {
    if (!$form_state->getValue('updated_solver_used')) {
      $form_state->setErrorByName('updated_solver_used', $this->t('Please select an updated solver.'));
      return;
    }

    if ($form_state->getValue('updated_solver_used') !== 'Other') {
      return;
    }

    $solver_text = trim((string) $form_state->getValue('updated_solver_used_text'));
    if ($solver_text === '') {
      $form_state->setErrorByName('updated_solver_used_text', $this->t('Solver used cannot be empty.'));
      return;
    }
    if (strlen($solver_text) > 100) {
      $form_state->setErrorByName('updated_solver_used_text', $this->t('Maximum character limit is 100.'));
      return;
    }
    if (strlen($solver_text) < 7) {
      $form_state->setErrorByName('updated_solver_used_text', $this->t('Minimum character limit is 7.'));
    }
  }

  /**
   * Returns a fallback message for states where the form should not display.
   */
  private function buildRedirectNotice($message, $route_name, $link_text) {
    return [
      'message' => [
        '#markup' => '<p>' . $message . '</p><p>' . Link::fromTextAndUrl($link_text, Url::fromRoute($route_name))->toString() . '</p>',
      ],
    ];
  }

}
