function update_folder_structure($old_version, $new_version, $old_type, $new_type, $project_title) {
    // Define the base path
    $base_path = upgradation_path();  // Adjust if different

    // Construct old and new paths
    $old_path = "$base_path/$old_version/$old_type/$project_title/files";
    $new_path = "$base_path/$new_version/$new_type/$project_title/files";

    // Check if the old path exists
    if (!file_exists($old_path)) {
        drupal_set_message(t('The specified folder does not exist.'), 'error');
        return FALSE;
    }

    // Create new directory if it doesn't exist
    if (!file_exists($new_path)) {
        mkdir($new_path, 0777, TRUE);
    }

    // Move files from old folder to new folder
    $files = scandir($old_path);
    foreach ($files as $file) {
        if ($file != '.' && $file != '..') {
            rename("$old_path/$file", "$new_path/$file");
        }
    }

    // Optionally remove the old folder if empty
    if (is_dir_empty($old_path)) {
        rmdir($old_path);
    }

    return TRUE;
}

/**
 * Helper function to check if a directory is empty.
 */
function is_dir_empty($dir) {
    if (!is_readable($dir)) return NULL; 
    $handle = opendir($dir);
    while (false !== ($entry = readdir($handle))) {
        if ($entry != "." && $entry != "..") {
            return FALSE;
        }
    }
    return TRUE;
}
