<?php

namespace Drupal\upgradation\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Url;

class UpgradationUploadAbstractCodeForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_upload_abstract_code_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $proposal = upgradation_load_approved_user_proposal($this->currentUser()->id());
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid proposal selected. Please try again.'));
      return $this->buildRedirectNotice(
        $this->t('No approved proposal is available for upload.'),
        'upgradation.abstract',
        $this->t('Back to submission summary')
      );
    }

    $submission = upgradation_get_submission_record($proposal->id);
    if ($submission && (int) $submission->is_submitted === 1) {
      $this->messenger()->addError($this->t('You have already submitted your case directory. For any query, please write to us.'));
      return $this->buildRedirectNotice(
        $this->t('Your case directory has already been submitted.'),
        'upgradation.abstract',
        $this->t('Back to submission summary')
      );
    }

    $current_abstract = default_value_for_uploaded_files('A', $proposal->id);
    $current_project = default_value_for_uploaded_files('S', $proposal->id);
    $current_run = default_value_for_uploaded_files('R', $proposal->id);

    $form['#attributes']['enctype'] = 'multipart/form-data';

    $form['project_title'] = [
      '#type' => 'item',
      '#title' => $this->t('Title of the Case Study Project'),
      '#markup' => Html::escape($proposal->project_title),
    ];

    $form['contributor_name'] = [
      '#type' => 'item',
      '#title' => $this->t('Contributor Name'),
      '#markup' => Html::escape($proposal->contributor_name),
    ];

    $form['upload_an_abstract'] = [
      '#type' => 'file',
      '#title' => $this->t('Upload an abstract of the project'),
      '#description' => $this->buildFileDescription($current_abstract->filename ?? $this->t('No file uploaded'), upgradation_allowed_extensions('A')),
    ];

    $form['upload_case_study_developed_process'] = [
      '#type' => 'file',
      '#title' => $this->t('Upload the Case Directory'),
      '#description' => $this->buildFileDescription($current_project->filename ?? $this->t('No file uploaded'), upgradation_allowed_extensions('S')),
    ];

    $form['all_run'] = [
      '#type' => 'file',
      '#title' => $this->t('Upload the All Run file'),
      '#description' => $this->buildFileDescription($current_run->filename ?? $this->t('No file uploaded'), upgradation_allowed_extensions('R')),
    ];

    $form['prop_id'] = [
      '#type' => 'hidden',
      '#value' => $proposal->id,
    ];

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Submit'),
    ];

    $form['cancel'] = [
      '#type' => 'item',
      '#markup' => Link::fromTextAndUrl($this->t('Cancel'), Url::fromRoute('upgradation.abstract'))->toString(),
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, FormStateInterface $form_state) {
    if (empty($_FILES['files'])) {
      $form_state->setErrorByName('upload_an_abstract', $this->t('Please upload the required files.'));
      return;
    }

    $required_fields = [
      'upload_an_abstract' => 'A',
      'upload_case_study_developed_process' => 'S',
      'all_run' => 'R',
    ];

    foreach ($required_fields as $field_name => $file_type) {
      if (empty($_FILES['files']['name'][$field_name])) {
        $form_state->setErrorByName($field_name, $this->t('This file is required.'));
        continue;
      }
      $this->validateUploadedFile($form_state, $field_name, $file_type);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $proposal = upgradation_load_proposal($form_state->getValue('prop_id'));
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid proposal selected. Please try again.'));
      $form_state->setRedirect('upgradation.abstract');
      return;
    }

    $directory = upgradation_ensure_proposal_directory($proposal);
    $submission = upgradation_get_submission_record($proposal->id);
    if (!$submission) {
      $submitted_abstract_id = \Drupal::database()->insert('csu_submitted_abstracts')
        ->fields([
          'proposal_id' => $proposal->id,
          'approver_uid' => 0,
          'abstract_approval_status' => 0,
          'abstract_upload_date' => time(),
          'abstract_approval_date' => 0,
          'is_submitted' => 1,
        ])
        ->execute();
      $this->messenger()->addStatus($this->t('Abstract uploaded successfully.'));
    }
    else {
      $submitted_abstract_id = $submission->id;
      \Drupal::database()->update('csu_submitted_abstracts')
        ->fields([
          'abstract_upload_date' => time(),
          'is_submitted' => 1,
        ])
        ->condition('proposal_id', $proposal->id)
        ->execute();
      $this->messenger()->addStatus($this->t('Abstract updated successfully.'));
    }

    \Drupal::database()->update('csu_proposal')
      ->fields([
        'is_submitted' => 1,
      ])
      ->condition('id', $proposal->id)
      ->execute();

    $field_map = [
      'upload_an_abstract' => 'A',
      'upload_case_study_developed_process' => 'S',
      'all_run' => 'R',
    ];

    foreach ($field_map as $field_name => $file_type) {
      $file_name = $_FILES['files']['name'][$field_name] ?? '';
      if (!$file_name) {
        continue;
      }

      $existing = upgradation_get_uploaded_file_record($proposal->id, $file_type);
      if ($existing && is_file($directory . $existing->filename)) {
        @unlink($directory . $existing->filename);
      }

      $destination = $directory . $file_name;
      if (file_exists($destination)) {
        @unlink($destination);
      }

      if (!move_uploaded_file($_FILES['files']['tmp_name'][$field_name], $destination)) {
        $this->messenger()->addError($this->t('Error uploading file: @file', ['@file' => $file_name]));
        continue;
      }

      $fields = [
        'submitted_abstract_id' => $submitted_abstract_id,
        'proposal_id' => $proposal->id,
        'uid' => (int) $this->currentUser()->id(),
        'approvar_uid' => 0,
        'filename' => $file_name,
        'filepath' => $file_name,
        'filemime' => mime_content_type($destination) ?: 'application/octet-stream',
        'filesize' => filesize($destination),
        'filetype' => $file_type,
        'timestamp' => time(),
      ];

      if ($existing) {
        \Drupal::database()->update('csu_submitted_abstracts_file')
          ->fields($fields)
          ->condition('proposal_id', $proposal->id)
          ->condition('filetype', $file_type)
          ->execute();
        $this->messenger()->addStatus($this->t('@file updated successfully.', ['@file' => $file_name]));
      }
      else {
        \Drupal::database()->insert('csu_submitted_abstracts_file')
          ->fields($fields)
          ->execute();
        $this->messenger()->addStatus($this->t('@file uploaded successfully.', ['@file' => $file_name]));
      }
    }

    $mail = upgradation_mail_settings();
    $langcode = $this->currentUser()->getPreferredLangcode() ?: \Drupal::languageManager()->getDefaultLanguage()->getId();
    $params['abstract_uploaded']['proposal_id'] = $proposal->id;
    $params['abstract_uploaded']['submitted_abstract_id'] = $submitted_abstract_id;
    $params['abstract_uploaded']['user_id'] = (int) $this->currentUser()->id();
    $params['abstract_uploaded']['headers'] = [
      'From' => $mail['from'],
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $mail['cc'],
      'Bcc' => $mail['bcc'],
    ];
    if (!upgradation_send_mail('upgradation', 'abstract_uploaded', $this->currentUser()->getEmail(), $langcode, $params, $mail['from'])) {
      $this->messenger()->addError($this->t('Error sending email message.'));
    }

    $form_state->setRedirect('upgradation.abstract');
  }

  /**
   * Builds the file description text.
   */
  private function buildFileDescription($current_file, $allowed_extensions) {
    return $this->t('Current file: @file. Allowed extensions: @extensions', [
      '@file' => (string) $current_file,
      '@extensions' => $allowed_extensions ?: $this->t('not configured'),
    ]);
  }

  /**
   * Validates an uploaded file field.
   */
  private function validateUploadedFile(FormStateInterface $form_state, $field_name, $file_type) {
    $file_name = $_FILES['files']['name'][$field_name] ?? '';
    if (!$file_name) {
      return;
    }

    $allowed_extensions = array_filter(array_map('trim', explode(',', upgradation_allowed_extensions($file_type))));
    $parts = explode('.', strtolower($file_name));
    $extension = end($parts);

    if ($allowed_extensions && !in_array($extension, $allowed_extensions, TRUE)) {
      $form_state->setErrorByName($field_name, $this->t('Only files with the following extensions can be uploaded: @extensions', ['@extensions' => implode(', ', $allowed_extensions)]));
    }

    if (($_FILES['files']['size'][$field_name] ?? 0) <= 0) {
      $form_state->setErrorByName($field_name, $this->t('File size cannot be zero.'));
    }

    if (!upgradation_check_valid_filename($file_name)) {
      $form_state->setErrorByName($field_name, $this->t('Invalid file name specified. Only letters, numbers, dots, dashes, and underscores are allowed.'));
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
