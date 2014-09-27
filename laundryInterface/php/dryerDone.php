<?php

	$fp = fopen('../data/xivelyFeed.json', 'w');
    fwrite($fp, "{\"version\":\"1.0.0\",\"datastreams\":[{\"id\":\"Dryer\",\"current_value\":\"0\"}]}");
    fclose($fp);
	
	$output = shell_exec("./scripts/sendXivelyFeed.sh");
    //echo "<pre>$output</pre>";

	$output = shell_exec("./scripts/textDryerDone.sh");

?>