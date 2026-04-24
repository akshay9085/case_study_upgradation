<?php

namespace Drupal\upgradation\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

class UpgradationSettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_settings_form';
  }

  /**
   * {@inheritdoc}
  */
  protected function getEditableConfigNames() {
    return ['upgradation.settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('upgradation.settings');

    $form['emails'] = [
      '#type' => 'textfield',
      '#title' => $this->t('(Bcc) Notification emails'),
      '#description' => $this->t('Specify email IDs for the Bcc header as a comma-separated list.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('upgraded_case_study_emails') ?: '',
    ];

    $form['cc_emails'] = [
      '#type' => 'textfield',
      '#title' => $this->t('(Cc) Notification emails'),
      '#description' => $this->t('Specify email IDs for the Cc header as a comma-separated list.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('upgraded_case_study_cc_emails') ?: '',
    ];

    $form['from_email'] = [
      '#type' => 'email',
      '#title' => $this->t('Outgoing from email address'),
      '#description' => $this->t('Email address displayed in the From field of outgoing messages.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('upgraded_case_study_from_email') ?: '',
    ];

    $form['extensions'] = [
      '#type' => 'details',
      '#title' => $this->t('Allowed file extensions'),
      '#open' => TRUE,
    ];

    $form['extensions']['abstract_upload'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Abstract upload extensions'),
      '#description' => $this->t('Comma-separated extensions allowed for abstract uploads.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('upgradation_abstract_upload_extensions') ?: '',
    ];

    $form['extensions']['upload_case_study_developed_process'] = [
      '#type' => 'textfield',
      '#title' => $this->t('Project file extensions'),
      '#description' => $this->t('Comma-separated extensions allowed for case directory uploads.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('upgraded_case_study_project_files_extensions') ?: '',
    ];

    $form['extensions']['all_run'] = [
      '#type' => 'textfield',
      '#title' => $this->t('All run file extensions'),
      '#description' => $this->t('Comma-separated extensions allowed for all run uploads.'),
      '#maxlength' => 255,
      '#required' => TRUE,
      '#default_value' => $config->get('all_run_extensions') ?: '',
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->configFactory->getEditable('upgradation.settings')
      ->set('upgraded_case_study_emails', trim($form_state->getValue('emails')))
      ->set('upgraded_case_study_cc_emails', trim($form_state->getValue('cc_emails')))
      ->set('upgraded_case_study_from_email', trim($form_state->getValue('from_email')))
      ->set('upgradation_abstract_upload_extensions', trim($form_state->getValue('abstract_upload')))
      ->set('upgraded_case_study_project_files_extensions', trim($form_state->getValue('upload_case_study_developed_process')))
      ->set('all_run_extensions', trim($form_state->getValue('all_run')))
      ->save();

    parent::submitForm($form, $form_state);
  }

}
