<?php

namespace Drupal\upgradation\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Url;

class UpgradationProposalStatusForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_proposal_status_form';
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
    $version = upgradation_get_version_name($proposal->version_id) ?: 'NA';
    $status = $this->formatStatus((int) $proposal->approval_status);
    $disapproval_reason = property_exists($proposal, 'message') ? $proposal->message : (property_exists($proposal, 'dissapproval_reason') ? $proposal->dissapproval_reason : '');
    if ($disapproval_reason === 'NULL') {
      $disapproval_reason = '';
    }

    $form['proposal_id'] = [
      '#type' => 'hidden',
      '#value' => $proposal->id,
    ];

    $form['contributor_name'] = [
      '#type' => 'item',
      '#title' => $this->t('Student name'),
      '#markup' => $account ? Link::fromTextAndUrl($proposal->name_title . ' ' . $proposal->contributor_name, Url::fromRoute('entity.user.canonical', ['user' => $proposal->uid]))->toString() : Html::escape($proposal->contributor_name),
    ];

    $form['student_email_id'] = [
      '#type' => 'item',
      '#title' => $this->t('Email'),
      '#markup' => Html::escape($account ? $account->getEmail() : ''),
    ];

    $form['university'] = [
      '#type' => 'item',
      '#title' => $this->t('University/Institute'),
      '#markup' => Html::escape($proposal->university),
    ];

    $form['faculty_name'] = [
      '#type' => 'item',
      '#title' => $this->t('Name of the faculty'),
      '#markup' => Html::escape($proposal->faculty_name ?: 'NA'),
    ];

    $form['faculty_department'] = [
      '#type' => 'item',
      '#title' => $this->t('Department of the faculty'),
      '#markup' => Html::escape($proposal->faculty_department ?: 'NA'),
    ];

    $form['faculty_email'] = [
      '#type' => 'item',
      '#title' => $this->t('Email of the faculty'),
      '#markup' => Html::escape($proposal->faculty_email ?: 'NA'),
    ];

    $form['country'] = [
      '#type' => 'item',
      '#title' => $this->t('Country'),
      '#markup' => Html::escape($proposal->country),
    ];

    $form['all_state'] = [
      '#type' => 'item',
      '#title' => $this->t('State'),
      '#markup' => Html::escape($proposal->state),
    ];

    $form['city'] = [
      '#type' => 'item',
      '#title' => $this->t('City'),
      '#markup' => Html::escape($proposal->city),
    ];

    $form['pincode'] = [
      '#type' => 'item',
      '#title' => $this->t('Pincode/Postal code'),
      '#markup' => Html::escape($proposal->pincode),
    ];

    $form['project_title'] = [
      '#type' => 'item',
      '#title' => $this->t('Title of the Case Study Project'),
      '#markup' => Html::escape($proposal->project_title),
    ];

    $form['version'] = [
      '#type' => 'item',
      '#title' => $this->t('Version used'),
      '#markup' => Html::escape($version),
    ];

    $form['simulation_type'] = [
      '#type' => 'item',
      '#title' => $this->t('Simulation Type'),
      '#markup' => Html::escape($proposal->simulation_type_id),
    ];

    $form['solver_used'] = [
      '#type' => 'item',
      '#title' => $this->t('Solver used'),
      '#markup' => Html::escape($proposal->solver_used),
    ];

    $form['proposal_status'] = [
      '#type' => 'item',
      '#title' => $this->t('Proposal Status'),
      '#markup' => Html::escape($status),
    ];

    if ((int) $proposal->approval_status === 1) {
      $form['completed'] = [
        '#type' => 'checkbox',
        '#title' => $this->t('Completed'),
        '#description' => $this->t('Check if the user has provided all the required files.'),
      ];
    }

    if ((int) $proposal->approval_status === 2 && $disapproval_reason) {
      $form['message'] = [
        '#type' => 'item',
        '#title' => $this->t('Reason for disapproval'),
        '#markup' => Html::escape($disapproval_reason),
      ];
    }

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
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $proposal = upgradation_load_proposal($form_state->getValue('proposal_id'));
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid proposal selected. Please try again.'));
      $form_state->setRedirect('upgradation.proposal_all');
      return;
    }

    if (!$form_state->getValue('completed')) {
      $form_state->setRedirect('upgradation.proposal_all');
      return;
    }

    \Drupal::database()->update('csu_proposal')
      ->fields([
        'approval_status' => 3,
        'actual_completion_date' => time(),
      ])
      ->condition('id', $proposal->id)
      ->execute();

    CreateReadmeFileUpgradedCaseStudyProject($proposal->id);

    $account = upgradation_load_user($proposal->uid);
    $mail = upgradation_mail_settings();
    $langcode = $account ? ($account->getPreferredLangcode() ?: \Drupal::languageManager()->getDefaultLanguage()->getId()) : \Drupal::languageManager()->getDefaultLanguage()->getId();
    $email_to = $account ? $account->getEmail() : '';
    $params['case_study_proposal_completed']['proposal_id'] = $proposal->id;
    $params['case_study_proposal_completed']['user_id'] = $proposal->uid;
    $params['case_study_proposal_completed']['headers'] = [
      'From' => $mail['from'],
      'MIME-Version' => '1.0',
      'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
      'Content-Transfer-Encoding' => '8Bit',
      'X-Mailer' => 'Drupal',
      'Cc' => $mail['cc'],
      'Bcc' => trim($this->currentUser()->getEmail() . ', ' . $mail['bcc'], ', '),
    ];
    if ($email_to && !upgradation_send_mail('case_study', 'case_study_proposal_completed', $email_to, $langcode, $params, $mail['from'])) {
      $this->messenger()->addError($this->t('Error sending email message.'));
    }

    $this->messenger()->addStatus($this->t('Congratulations! CFD Case Study proposal has been marked as completed. The user has been notified.'));
    $form_state->setRedirect('upgradation.proposal_all');
  }

  /**
   * Returns a status label.
   */
  private function formatStatus($status) {
    switch ($status) {
      case 0:
        return (string) $this->t('Pending');

      case 1:
        return (string) $this->t('Approved');

      case 2:
        return (string) $this->t('Dis-approved');

      case 3:
        return (string) $this->t('Completed');

      case 5:
        return (string) $this->t('On Hold');

      default:
        return (string) $this->t('Unknown');
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
