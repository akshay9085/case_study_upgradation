<?php

namespace Drupal\upgradation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

class UpgradationAbstractBulkApprovalForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_abstract_bulk_approval_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $options = _bulk_list_of_upgradation();
    $selected = (int) $form_state->getValue('case_study_project');
    if (!$selected) {
      $selected = 0;
    }

    $form['wrapper'] = [
      '#type' => 'container',
      '#prefix' => '<div id="upgradation-bulk-wrapper">',
      '#suffix' => '</div>',
    ];

    $form['wrapper']['case_study_project'] = [
      '#type' => 'select',
      '#title' => $this->t('Title of the Case Study Project'),
      '#options' => $options,
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => '::updateCaseStudyDetails',
        'wrapper' => 'upgradation-bulk-wrapper',
      ],
    ];

    $form['wrapper']['details'] = [
      '#type' => 'item',
      '#markup' => $selected ? _case_study_details($selected) : '',
    ];

    $form['wrapper']['case_study_actions'] = [
      '#type' => 'select',
      '#title' => $this->t('Please select an action for this case study project'),
      '#options' => _bulk_list_case_study_actions(),
      '#default_value' => (int) $form_state->getValue('case_study_actions'),
      '#states' => [
        'invisible' => [
          ':input[name="case_study_project"]' => ['value' => '0'],
        ],
      ],
    ];

    $form['wrapper']['message'] = [
      '#type' => 'textarea',
      '#title' => $this->t('Reason for resubmission or disapproval'),
      '#states' => [
        'visible' => [
          [
            ':input[name="case_study_actions"]' => ['value' => '2'],
          ],
          'or',
          [
            ':input[name="case_study_actions"]' => ['value' => '3'],
          ],
        ],
      ],
    ];

    $form['wrapper']['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Submit'),
    ];

    return $form;
  }

  /**
   * Ajax callback.
   */
  public function updateCaseStudyDetails(array &$form, FormStateInterface $form_state) {
    return $form['wrapper'];
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $proposal_id = (int) $form_state->getValue('case_study_project');
    $action = (int) $form_state->getValue('case_study_actions');
    $message = trim((string) $form_state->getValue('message'));
    $proposal = upgradation_load_proposal($proposal_id);
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid case study project selected.'));
      return;
    }

    $account = upgradation_load_user($proposal->uid);
    $langcode = $account ? ($account->getPreferredLangcode() ?: \Drupal::languageManager()->getDefaultLanguage()->getId()) : \Drupal::languageManager()->getDefaultLanguage()->getId();
    $mail = upgradation_mail_settings();
    $site_name = upgradation_site_name();

    switch ($action) {
      case 1:
        if (!$this->currentUser()->hasPermission('Case Study bulk manage abstract')) {
          $this->messenger()->addError($this->t('You do not have permission to approve submitted projects.'));
          return;
        }

        \Drupal::database()->update('csu_submitted_abstracts')
          ->fields([
            'abstract_approval_status' => 1,
            'is_submitted' => 1,
            'approver_uid' => (int) $this->currentUser()->id(),
          ])
          ->condition('proposal_id', $proposal_id)
          ->execute();

        \Drupal::database()->update('csu_submitted_abstracts_file')
          ->fields([
            'file_approval_status' => 1,
            'approvar_uid' => (int) $this->currentUser()->id(),
          ])
          ->condition('proposal_id', $proposal_id)
          ->execute();

        $this->messenger()->addStatus($this->t('Approved Case Study Project. Use the status page to mark it as completed when the work is done.'));
        $form_state->setRedirect('upgradation.proposal_status_form', ['proposal_id' => $proposal_id]);
        return;

      case 2:
        if (!$this->currentUser()->hasPermission('Case Study bulk manage abstract')) {
          $this->messenger()->addError($this->t('You do not have permission to resubmit submitted projects.'));
          return;
        }

        \Drupal::database()->update('csu_submitted_abstracts')
          ->fields([
            'abstract_approval_status' => 0,
            'is_submitted' => 0,
            'approver_uid' => (int) $this->currentUser()->id(),
          ])
          ->condition('proposal_id', $proposal_id)
          ->execute();

        \Drupal::database()->update('csu_submitted_abstracts_file')
          ->fields([
            'file_approval_status' => 0,
            'approvar_uid' => (int) $this->currentUser()->id(),
          ])
          ->condition('proposal_id', $proposal_id)
          ->execute();

        \Drupal::database()->update('csu_proposal')
          ->fields([
            'is_submitted' => 0,
            'approver_uid' => (int) $this->currentUser()->id(),
          ])
          ->condition('id', $proposal_id)
          ->execute();

        $params['standard']['subject'] = $this->t('[!site_name][Case Study Project] Your uploaded case study project has been marked as pending', ['!site_name' => $site_name]);
        $params['standard']['body'] = [
          $this->t("Dear @name,\n\nKindly resubmit the project files for the project: @title.\n\nReason for resubmission: @message\n\nBest wishes,\n\n@site_name Team,\nFOSSEE, IIT Bombay", [
            '@name' => $proposal->contributor_name,
            '@title' => $proposal->project_title,
            '@message' => $message ?: $this->t('No reason provided'),
            '@site_name' => $site_name,
          ]),
        ];
        $params['standard']['headers'] = [
          'From' => $mail['from'],
          'MIME-Version' => '1.0',
          'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
          'Content-Transfer-Encoding' => '8Bit',
          'X-Mailer' => 'Drupal',
          'Cc' => $mail['cc'],
          'Bcc' => $mail['bcc'],
        ];

        if ($account && !upgradation_send_mail('scilab_case_study', 'standard', $account->getEmail(), $langcode, $params, $mail['from'])) {
          $this->messenger()->addError($this->t('Error sending email message.'));
        }

        $this->messenger()->addStatus($this->t('Project files have been marked for resubmission.'));
        return;

      case 3:
        if (strlen($message) < 30) {
          $form_state->setErrorByName('message', $this->t('Please mention the reason for disapproval. A minimum of 30 characters is required.'));
          return;
        }
        if (!$this->currentUser()->hasPermission('Upgradation bulk delete abstract')) {
          $this->messenger()->addError($this->t('You do not have permission to disapprove and delete an entire project.'));
          return;
        }
        if (!upgradation_abstract_delete_project($proposal_id)) {
          $this->messenger()->addError($this->t('Error dis-approving and deleting the entire case study project.'));
          return;
        }

        $params['standard']['subject'] = $this->t('[!site_name][Case Study Project] Your uploaded case study project has been marked as dis-approved', ['!site_name' => $site_name]);
        $params['standard']['body'] = [
          $this->t("Dear @name,\n\nYour uploaded case study project files for the case study project title @title have been marked as dis-approved.\n\nReason for dis-approval: @message\n\nBest wishes,\n\n@site_name Team,\nFOSSEE, IIT Bombay", [
            '@name' => $proposal->contributor_name,
            '@title' => $proposal->project_title,
            '@message' => $message,
            '@site_name' => $site_name,
          ]),
        ];
        $params['standard']['headers'] = [
          'From' => $mail['from'],
          'MIME-Version' => '1.0',
          'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
          'Content-Transfer-Encoding' => '8Bit',
          'X-Mailer' => 'Drupal',
          'Cc' => $mail['cc'],
          'Bcc' => $mail['bcc'],
        ];

        if ($account && !upgradation_send_mail('scilab_case_study', 'standard', $account->getEmail(), $langcode, $params, $mail['from'])) {
          $this->messenger()->addError($this->t('Error sending email message.'));
        }

        $this->messenger()->addStatus($this->t('Dis-approved and deleted the entire case study project.'));
        return;
    }

    $this->messenger()->addError($this->t('Please select an action.'));
  }

}
