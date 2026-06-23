<?php

namespace Drupal\upgradation\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Url;

class UpgradationProposalEditForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_proposal_edit_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, $proposal_id = NULL) {
    $proposal = upgradation_load_proposal($proposal_id);
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid proposal selected. Please try again.'));
      return $this->buildRedirectNotice(
        $this->t('The selected proposal could not be loaded.'),
        'upgradation.proposal_all',
        $this->t('Back to proposal list')
      );
    }

    $account = upgradation_load_user($proposal->uid);
    $selected_version = $form_state->getValue('version') ?: $proposal->version_id;
    $solver_options = _csu_list_of_solvers($selected_version);
    $version_label = upgradation_get_version_name($proposal->version_id) ?: 'NA';

    $form['proposal_id'] = [
      '#type' => 'hidden',
      '#value' => $proposal->id,
    ];

    $form['project_title_value'] = [
      '#type' => 'hidden',
      '#value' => $proposal->project_title,
    ];

    $form['simulation_type_value'] = [
      '#type' => 'hidden',
      '#value' => $proposal->simulation_type_id,
    ];

    $form['name_title'] = [
      '#type' => 'select',
      '#title' => $this->t('Title'),
      '#options' => [
        'Dr' => 'Dr',
        'Prof' => 'Prof',
        'Mr' => 'Mr',
        'Ms' => 'Ms',
      ],
      '#default_value' => $proposal->name_title,
      '#required' => TRUE,
    ];

    $form['contributor_name'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Name of the proposer'),
      '#maxlength' => 50,
      '#default_value' => $proposal->contributor_name,
      '#required' => TRUE,
    ];

    $form['student_email_id'] = [
      '#type' => 'item',
      '#title' => $this->t('Email'),
      '#markup' => Html::escape($account ? $account->getEmail() : ''),
    ];

    $form['university'] = [
      '#type' => 'textfield',
      '#title' => $this->t('University/Institute'),
      '#maxlength' => 200,
      '#default_value' => $proposal->university,
      '#required' => TRUE,
    ];

    $form['institute'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Institute'),
      '#maxlength' => 200,
      '#default_value' => $proposal->institute,
      '#required' => TRUE,
    ];

    $form['faculty_name'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Name of the faculty'),
      '#maxlength' => 50,
      '#default_value' => $proposal->faculty_name,
    ];

    $form['faculty_department'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Department of the faculty'),
      '#maxlength' => 50,
      '#default_value' => $proposal->faculty_department,
    ];

    $form['faculty_email'] = [
      '#type' => 'email',
      '#title' => $this->t('Email id of the faculty'),
      '#maxlength' => 255,
      '#default_value' => $proposal->faculty_email,
    ];

    $form['country'] = [
      '#type' => 'select',
      '#title' => $this->t('Country'),
      '#options' => [
        'India' => 'India',
        'Others' => 'Others',
      ],
      '#default_value' => $proposal->country === 'India' ? 'India' : 'Others',
      '#required' => TRUE,
    ];

    $form['other_country'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Other than India'),
      '#default_value' => $proposal->country !== 'India' ? $proposal->country : '',
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'Others'],
        ],
      ],
    ];

    $form['other_state'] = [
      '#type' => 'textfield',
      '#title' => $this->t('State other than India'),
      '#default_value' => $proposal->country !== 'India' ? $proposal->state : '',
      '#states' => [
        'visible' => [
          ':input[name="country"]' => ['value' => 'Others'],
        ],
      ],
    ];

    $form['other_city'] = [
      '#type' => 'textfield',
      '#title' => $this->t('City other than India'),
      '#default_value' => $proposal->country !== 'India' ? $proposal->city : '',
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
      '#default_value' => $proposal->country === 'India' ? $proposal->state : '',
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
      '#default_value' => $proposal->country === 'India' ? $proposal->city : '',
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
      '#default_value' => $proposal->pincode,
    ];

    $form['project_title'] = [
      '#type' => 'item',
      '#title' => $this->t('Title of the Case Study Project'),
      '#markup' => Html::escape($proposal->project_title),
    ];

    $form['version'] = [
      '#type' => 'select',
      '#title' => $this->t('Version used'),
      '#options' => _csu_list_of_versions(),
      '#default_value' => $selected_version,
      '#ajax' => [
        'callback' => '::updateSolverSelection',
        'wrapper' => 'solver-selection-wrapper',
      ],
    ];

    $form['simulation_type'] = [
      '#type' => 'item',
      '#title' => $this->t('Simulation Type used'),
      '#markup' => Html::escape($proposal->simulation_type_id),
    ];

    $form['solver_wrapper'] = [
      '#type' => 'container',
      '#prefix' => '<div id="solver-selection-wrapper">',
      '#suffix' => '</div>',
    ];

    $form['solver_wrapper']['solver_used'] = [
      '#type' => 'select',
      '#title' => $this->t('Select the solver to be used'),
      '#options' => $solver_options,
      '#default_value' => $form_state->getValue('solver_used') ?: $proposal->solver_used,
      '#required' => TRUE,
    ];

    $form['solver_wrapper']['solver_used_text'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Enter the solver to be used'),
      '#description' => $this->t('Maximum character limit is 100.'),
      '#default_value' => $proposal->solver_used && !isset($solver_options[$proposal->solver_used]) ? $proposal->solver_used : '',
      '#states' => [
        'visible' => [
          ':input[name="solver_used"]' => ['value' => 'Other'],
        ],
      ],
    ];

    $form['date_of_proposal'] = [
      '#type' => 'item',
      '#title' => $this->t('Date of Proposal'),
      '#markup' => date('d/m/Y', (int) $proposal->creation_date),
    ];

    $form['delete_proposal'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Delete Proposal'),
    ];

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Submit'),
    ];

    $form['cancel'] = [
      '#type' => 'item',
      '#markup' => Link::fromTextAndUrl($this->t('Cancel'), Url::fromRoute('upgradation.proposal_all'))->toString(),
    ];

    return $form;
  }

  /**
   * Ajax callback for solver selection.
   */
  public function updateSolverSelection(array &$form, FormStateInterface $form_state) {
    return $form['solver_wrapper'];
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    if ($form_state->getValue('country') === 'Others') {
      if (!trim((string) $form_state->getValue('other_country'))) {
        $form_state->setErrorByName('other_country', $this->t('Enter country name.'));
      }
      else {
        $form_state->setValue('country', trim((string) $form_state->getValue('other_country')));
      }

      if (!trim((string) $form_state->getValue('other_state'))) {
        $form_state->setErrorByName('other_state', $this->t('Enter state name.'));
      }
      else {
        $form_state->setValue('all_state', trim((string) $form_state->getValue('other_state')));
      }

      if (!trim((string) $form_state->getValue('other_city'))) {
        $form_state->setErrorByName('other_city', $this->t('Enter city name.'));
      }
      else {
        $form_state->setValue('city', trim((string) $form_state->getValue('other_city')));
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

    $this->validateSolverField($form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $proposal = upgradation_load_proposal($form_state->getValue('proposal_id'));
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid proposal selected. Please try again.'));
      $form_state->setRedirect('upgradation.proposal_all');
      return;
    }

    if ($form_state->getValue('delete_proposal')) {
      $this->deleteProposal($proposal, $form_state);
      return;
    }

    $project_title = $form_state->getValue('project_title_value');
    $proposer_name = $form_state->getValue('name_title') . ' ' . $form_state->getValue('contributor_name');
    $directory_name = _csu_dir_name($project_title, $proposer_name);
    if (!CSU_RenameDir($proposal->id, $directory_name)) {
      return;
    }

    $solver = $form_state->getValue('solver_used') === 'Other' ? $form_state->getValue('solver_used_text') : $form_state->getValue('solver_used');
    \Drupal::database()->update('csu_proposal')
      ->fields([
        'name_title' => $form_state->getValue('name_title'),
        'contributor_name' => trim((string) $form_state->getValue('contributor_name')),
        'university' => $form_state->getValue('university'),
        'institute' => $form_state->getValue('institute'),
        'faculty_name' => $form_state->getValue('faculty_name'),
        'faculty_department' => $form_state->getValue('faculty_department'),
        'faculty_email' => $form_state->getValue('faculty_email'),
        'city' => $form_state->getValue('city'),
        'pincode' => $form_state->getValue('pincode'),
        'state' => $form_state->getValue('all_state'),
        'country' => $form_state->getValue('country'),
        'project_title' => $project_title,
        'version_id' => $form_state->getValue('version'),
        'simulation_type_id' => $form_state->getValue('simulation_type_value'),
        'solver_used' => $solver,
        'directory_name' => $directory_name,
      ])
      ->condition('id', $proposal->id)
      ->execute();

    $this->messenger()->addStatus($this->t('Proposal updated.'));
    $form_state->setRedirect('upgradation.proposal_all');
  }

  /**
   * Validates solver fields.
   */
  private function validateSolverField(FormStateInterface $form_state) {
    if (!$form_state->getValue('solver_used')) {
      $form_state->setErrorByName('solver_used', $this->t('Please select a solver.'));
      return;
    }

    if ($form_state->getValue('solver_used') !== 'Other') {
      return;
    }

    $solver_text = trim((string) $form_state->getValue('solver_used_text'));
    if ($solver_text === '') {
      $form_state->setErrorByName('solver_used_text', $this->t('Solver used cannot be empty.'));
      return;
    }
    if (strlen($solver_text) > 100) {
      $form_state->setErrorByName('solver_used_text', $this->t('Maximum character limit is 100.'));
      return;
    }
    if (strlen($solver_text) < 7) {
      $form_state->setErrorByName('solver_used_text', $this->t('Minimum character limit is 7.'));
    }
  }

  /**
   * Deletes a proposal and notifies the owner.
   */
  private function deleteProposal($proposal, FormStateInterface $form_state) {
    $account = upgradation_load_user($proposal->uid);
    $mail = upgradation_mail_settings();
    $langcode = $account ? ($account->getPreferredLangcode() ?: \Drupal::languageManager()->getDefaultLanguage()->getId()) : \Drupal::languageManager()->getDefaultLanguage()->getId();

    $params['case_study_proposal_deleted']['proposal_id'] = $proposal->id;
    $params['case_study_proposal_deleted']['user_id'] = $proposal->uid;
    $params['case_study_proposal_deleted']['headers'] = [
      'From' => $mail['from'],
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $mail['cc'],
      'Bcc' => $mail['bcc'],
    ];
    if ($account && !upgradation_send_mail('upgradation', 'case_study_proposal_deleted', $account->getEmail(), $langcode, $params, $mail['from'])) {
      $this->messenger()->addError($this->t('Error sending email message.'));
    }

    csu_rrmdir_project($proposal->id);
    \Drupal::database()->delete('csu_submitted_abstracts_file')->condition('proposal_id', $proposal->id)->execute();
    \Drupal::database()->delete('csu_submitted_abstracts')->condition('proposal_id', $proposal->id)->execute();
    \Drupal::database()->delete('csu_proposal')->condition('id', $proposal->id)->execute();

    $this->messenger()->addStatus($this->t('Proposal deleted.'));
    $form_state->setRedirect('upgradation.proposal_all');
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
