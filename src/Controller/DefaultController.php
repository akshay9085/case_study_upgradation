<?php

namespace Drupal\upgradation\Controller;

use Drupal\Component\Utility\Html;
use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Link;
use Drupal\Core\Url;
use Symfony\Component\HttpFoundation\BinaryFileResponse;
use Symfony\Component\HttpFoundation\RedirectResponse;
use Symfony\Component\HttpFoundation\ResponseHeaderBag;
use ZipArchive;

class DefaultController extends ControllerBase {

  /**
   * Lists completed case studies.
   */
  public function upgradation_completed_proposals_all() {
    $header = [
      $this->t('No'),
      $this->t('Case Study Project'),
      $this->t('Contributor Name'),
      $this->t('University / Institute'),
      $this->t('Year of Completion'),
    ];

    $rows = [];
    $result = \Drupal::database()
      ->select('csu_proposal', 'cp')
      ->fields('cp')
      ->condition('approval_status', 3)
      ->orderBy('actual_completion_date', 'DESC')
      ->execute();

    $index = 1;
    while ($proposal = $result->fetchObject()) {
      $project_link = Link::fromTextAndUrl($proposal->project_title, Url::fromRoute('upgradation.run_form', ['proposal_id' => $proposal->id]))->toString();
      $project_markup = $project_link . '<br><strong>(' . $this->t('Solver used: @solver', ['@solver' => $proposal->solver_used]) . ')</strong>';

      $rows[] = [
        $index,
        ['data' => ['#markup' => $project_markup]],
        Html::escape($proposal->contributor_name),
        Html::escape($proposal->university),
        date('Y', (int) $proposal->actual_completion_date),
      ];
      $index++;
    }

    return [
      'intro' => [
        '#markup' => '<p>' . $this->t('Work has been completed for the following case studies. We welcome your contributions.') . '</p><hr>',
      ],
      'table' => [
        '#type' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => $this->t('No completed case studies are available.'),
      ],
    ];
  }

  /**
   * Lists in-progress case studies.
   */
  public function upgradation_progress_all() {
    $header = [
      $this->t('No'),
      $this->t('Case Study Project'),
      $this->t('Contributor Name'),
      $this->t('Institute / University'),
      $this->t('Year'),
    ];

    $rows = [];
    $result = \Drupal::database()
      ->select('csu_proposal', 'cp')
      ->fields('cp')
      ->condition('approval_status', 1)
      ->condition('is_completed', 0)
      ->execute();

    $index = 1;
    while ($proposal = $result->fetchObject()) {
      $rows[] = [
        $index,
        Html::escape($proposal->project_title),
        Html::escape($proposal->contributor_name),
        Html::escape($proposal->university),
        $proposal->approval_date ? date('Y', (int) $proposal->approval_date) : $this->t('NA'),
      ];
      $index++;
    }

    return [
      'intro' => [
        '#markup' => '<p>' . $this->t('Work is in progress for the following case studies under the case study project.') . '</p><hr>',
      ],
      'table' => [
        '#type' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => $this->t('No in-progress case studies are available.'),
      ],
    ];
  }

  /**
   * Downloads a full uploaded project archive.
   */
  public function upgradation_download_full_project($proposal_id = 0) {
    $proposal = upgradation_load_proposal($proposal_id);
    if (!$proposal) {
      $this->messenger()->addError($this->t('Invalid case study proposal.'));
      return $this->redirect('upgradation.run_form');
    }

    $project_directory = upgradation_get_proposal_absolute_path($proposal);
    $files = upgradation_get_uploaded_file_records($proposal_id);
    if (!$files || !is_dir($project_directory)) {
      $this->messenger()->addError($this->t('There are no case study files available to download.'));
      return $this->redirect('upgradation.run_form', ['proposal_id' => $proposal_id]);
    }

    if (!class_exists(ZipArchive::class)) {
      $this->messenger()->addError($this->t('The ZipArchive extension is not available on this server.'));
      return $this->redirect('upgradation.run_form', ['proposal_id' => $proposal_id]);
    }

    $zip_filename = upgradation_path() . 'zip-' . time() . '-' . mt_rand(0, 999999) . '.zip';
    $zip = new ZipArchive();
    $zip_status = $zip->open($zip_filename, ZipArchive::CREATE | ZipArchive::OVERWRITE);
    if ($zip_status !== TRUE) {
      $this->messenger()->addError($this->t('The project archive could not be created.'));
      return $this->redirect('upgradation.run_form', ['proposal_id' => $proposal_id]);
    }

    $archive_root = upgradation_get_proposal_relative_path($proposal);
    foreach ($files as $file) {
      $absolute_file = $project_directory . $file->filepath;
      if (is_file($absolute_file)) {
        $zip->addFile($absolute_file, $archive_root . str_replace(' ', '_', basename($file->filename)));
      }
    }
    $zip->close();

    if (!file_exists($zip_filename) || filesize($zip_filename) === 0) {
      @unlink($zip_filename);
      $this->messenger()->addError($this->t('There are no case study files available to download.'));
      return $this->redirect('upgradation.run_form', ['proposal_id' => $proposal_id]);
    }

    $download_name = str_replace(' ', '_', $proposal->project_title) . '.zip';
    $response = new BinaryFileResponse($zip_filename);
    $response->setContentDisposition(ResponseHeaderBag::DISPOSITION_ATTACHMENT, $download_name);
    $response->deleteFileAfterSend(TRUE);
    return $response;
  }

  /**
   * Lists all proposals for management.
   */
  public function upgradation_proposal_all() {
    $header = [
      $this->t('Date of Submission'),
      $this->t('Student Name'),
      $this->t('Title of the case-study project'),
      $this->t('Date of Approval'),
      $this->t('Date of Project Completion'),
      $this->t('Status'),
      $this->t('Action'),
    ];

    $rows = [];
    $result = \Drupal::database()
      ->select('csu_proposal', 'cp')
      ->fields('cp')
      ->orderBy('id', 'DESC')
      ->execute();

    while ($proposal = $result->fetchObject()) {
      $student_link = Link::fromTextAndUrl($proposal->contributor_name, Url::fromRoute('entity.user.canonical', ['user' => $proposal->uid]))->toString();
      $status_link = Link::fromTextAndUrl($this->t('Status'), Url::fromRoute('upgradation.proposal_status_form', ['proposal_id' => $proposal->id]))->toString();
      $edit_link = Link::fromTextAndUrl($this->t('Edit'), Url::fromRoute('upgradation.proposal_edit_form', ['proposal_id' => $proposal->id]))->toString();

      $rows[] = [
        date('d-m-Y', (int) $proposal->creation_date),
        ['data' => ['#markup' => $student_link]],
        Html::escape($proposal->project_title),
        $proposal->approval_date ? date('d-m-Y', (int) $proposal->approval_date) : $this->t('Not Approved'),
        $proposal->actual_completion_date ? date('d-m-Y', (int) $proposal->actual_completion_date) : $this->t('Not Completed'),
        $this->formatStatus((int) $proposal->approval_status),
        ['data' => ['#markup' => $status_link . ' | ' . $edit_link]],
      ];
    }

    return [
      '#type' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => $this->t('There are no proposals.'),
    ];
  }

  /**
   * Lists proposals whose uploaded files can be edited.
   */
  public function upgradation_proposal_edit_file_all() {
    $header = [
      $this->t('Date of Submission'),
      $this->t('Student Name'),
      $this->t('Title of the case-study project'),
      $this->t('Date of Approval'),
      $this->t('Date of Project Completion'),
      $this->t('Status'),
      $this->t('Action'),
    ];

    $rows = [];
    $result = \Drupal::database()
      ->select('csu_proposal', 'cp')
      ->fields('cp')
      ->condition('approval_status', 1)
      ->orderBy('id', 'DESC')
      ->execute();

    while ($proposal = $result->fetchObject()) {
      $student_link = Link::fromTextAndUrl($proposal->contributor_name, Url::fromRoute('entity.user.canonical', ['user' => $proposal->uid]))->toString();
      $edit_link = Link::fromTextAndUrl($this->t('Edit'), Url::fromRoute('upgradation.edit_upload_abstract_code_form', ['proposal_id' => $proposal->id]))->toString();

      $rows[] = [
        date('d-m-Y', (int) $proposal->creation_date),
        ['data' => ['#markup' => $student_link]],
        Html::escape($proposal->project_title),
        $proposal->approval_date ? date('d-m-Y', (int) $proposal->approval_date) : $this->t('Not Approved'),
        $proposal->actual_completion_date ? date('d-m-Y', (int) $proposal->actual_completion_date) : $this->t('Not Completed'),
        $this->formatStatus((int) $proposal->approval_status),
        ['data' => ['#markup' => $edit_link]],
      ];
    }

    return [
      '#type' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => $this->t('There are no approved proposals with editable files.'),
    ];
  }

  /**
   * Shows the current user's uploaded file state.
   */
  public function upgradation_abstract() {
    $proposal = upgradation_get_proposal();
    if (!$proposal) {
      return new RedirectResponse(Url::fromRoute('<front>', [], ['absolute' => TRUE])->toString());
    }

    $files = upgradation_get_uploaded_file_records($proposal->id);
    $submitted = \Drupal::database()
      ->select('csu_submitted_abstracts', 'sa')
      ->fields('sa', ['is_submitted'])
      ->condition('proposal_id', $proposal->id)
      ->range(0, 1)
      ->execute()
      ->fetchField();

    $link_markup = '';
    if (!$submitted) {
      $link_markup = Link::fromTextAndUrl($this->t('Upload Case Directory'), Url::fromRoute('upgradation.upload_abstract_code_form'))->toString();
    }

    $markup = '<div class="upgradation-abstract-summary">';
    $markup .= '<p><strong>' . $this->t('Contributor Name') . ':</strong><br>' . Html::escape($proposal->name_title . ' ' . $proposal->contributor_name) . '</p>';
    $markup .= '<p><strong>' . $this->t('Title of the Case Study Project') . ':</strong><br>' . Html::escape($proposal->project_title) . '</p>';
    $markup .= '<p><strong>' . $this->t('Uploaded abstract of the project') . ':</strong><br>' . Html::escape($files['A']->filename ?? $this->t('File not uploaded')) . '</p>';
    $markup .= '<p><strong>' . $this->t('Uploaded Case Directory') . ':</strong><br>' . Html::escape($files['S']->filename ?? $this->t('File not uploaded')) . '</p>';
    $markup .= '<p><strong>' . $this->t('Uploaded All Run File') . ':</strong><br>' . Html::escape($files['R']->filename ?? $this->t('File not uploaded')) . '</p>';
    if ($link_markup) {
      $markup .= '<p>' . $link_markup . '</p>';
    }
    $markup .= '</div>';

    return ['#markup' => $markup];
  }

  /**
   * Formats an approval status label.
   */
  private function formatStatus($status) {
    switch ($status) {
      case 0:
        return $this->t('Pending');

      case 1:
        return $this->t('Approved');

      case 2:
        return $this->t('Dis-approved');

      case 3:
        return $this->t('Completed');

      case 5:
        return $this->t('On Hold');

      default:
        return $this->t('Unknown');
    }
  }

}
