<?php

/**
 * Expose the last edited field value as a context condition.
 */
class context_node_edited_since_context_condition_date_since extends context_condition {
  function condition_values() {
    $values = array();
    $values['week'] = 'week';
    $values['month'] = 'month';
    return $values;
  }

  function execute_old($node) {
    // Grab the value this node has stored for our field.
    // dpr($node);
    if ($items = field_get_items('node', $node, 'field_list', $node->language)) {

      // See if any of the field's values satisfy the condition.
      foreach ($items as $item) {
        foreach ($this->get_contexts($item['value']) as $context) {
          $this->condition_met($context, $item['value']);

        }
      }
    }
  }

  function execute($node) {
    $user_id = $node->revision_uid;
    $content_type = $node->type;
    echo('nu de waarden(user"' . $user_id . '"): ');
    // dpr($this->get_contexts('week'));
    // dpr($this->fetch_from_context());
    echo 'content type: ' . $content_type;

    $last_edit_date = db_query("SELECT r.timestamp
                                FROM {node_revision} r
                                INNER JOIN {node} n
                                ON r.nid = n.nid
                                WHERE r.uid = :user AND n.type = :content_type
                                ORDER BY r.timestamp DESC
                                LIMIT 1",
            array(':user' => $user_id,
            ':content_type' => $content_type,
            ))->fetchField();
    echo "bewerk datum: " . $last_edit_date;
    if($this->get_contexts('month')) {
      echo "maand is WAAR.";
      $month_ago = strtotime('-1 month');
      if ($month_ago > $last_edit_date ) {
        // Current content type has not been edited by current user for more
        // than a month.
        foreach ($this->get_contexts('month') as $context) {
            $this->condition_met($context, 'month');
        }
      }
    } elseif ($this->get_contexts('week')) {
      echo "Week is waar";
      // Context node edited since
      // $dbvraag = db_query("SELECT timestamp FROM {node_revision} where uid = :user ORDER BY timestamp DESC LIMIT 1",
          // array(':user' => $user_id))->fetchField();
      $week_ago = strtotime('- 1 week');

      if ($week_ago > $last_edit_date ) {
        // Current content type has not been edited by current user for more
        // than a week.
        foreach ($this->get_contexts('week') as $context) {
            $this->condition_met($context, 'week');
          }
        // $this->condition_met($this->get_contexts('week'));
      }
    }
  }
}