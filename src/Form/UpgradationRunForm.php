<?php

namespace Drupal\upgradation\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;

class UpgradationRunForm extends FormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'upgradation_run_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state, $proposal_id = 0) {
    $options = _list_of_upgraded_case_study();
    $selected = (int) $form_state->getValue('case_study');
    if (!$selected && !empty($proposal_id) && isset($options[$proposal_id])) {
      $selected = (int) $proposal_id;
    }
    if (!$selected) {
      $selected = 0;
    }

    $form['wrapper'] = [
      '#type' => 'container',
      '#prefix' => '<div id="upgradation-run-wrapper">',
      '#suffix' => '</div>',
    ];

    $form['wrapper']['case_study'] = [
      '#type' => 'select',
      '#title' => $this->t('Title of the case study'),
      '#options' => $options,
      '#default_value' => $selected,
      '#ajax' => [
        'callback' => '::updateDetails',
        'wrapper' => 'upgradation-run-wrapper',
      ],
    ];

    $form['wrapper']['case_study_details'] = [
      '#type' => 'item',
      '#markup' => $selected ? _upgraded_case_study_details($selected) : '<div id="ajax_upgraded_case_study_details"></div>',
    ];

    return $form;
  }

  /**
   * Ajax callback for the details area.
   */
  public function updateDetails(array &$form, FormStateInterface $form_state) {
    return $form['wrapper'];
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
  }

}
