<?php
$phpcode = qw_query_clean_export(qw_query_export($query_id));
?>
<textarea id="export-query"><?php print qw_textarea($phpcode); ?></textarea>