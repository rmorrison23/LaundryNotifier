<?php
	
    $washer = $_POST['washer'];
    $dryer = $_POST['dryer'];
    $both = $_POST['both'];
    $notifier = $_POST['notifier'];

    $washOnArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Washer',
                'current_value' => '100'
            ])
        );

    $washOffArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Washer',
                'current_value' => '0'
            ])
        );

    $dryOnArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Dryer',
                'current_value' => '100'
            ])
        );

    $dryOffArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Dryer',
                'current_value' => '0'
            ])
        );

    $bothOnArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Both',
                'current_value' => '100'
            ])
        );

     $bothOffArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Both',
                'current_value' => '0'
            ])
        );

     $notifierOnArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Notifier',
                'current_value' => '100'
            ])
        );

     $notifierOffArray = array(
            'version' => '1.0.0',
            'datastreams' => array([
                'id' => 'Notifier',
                'current_value' => '0'
            ])
        );

    function setWasherOn() {

        global $washOnArray, $dryOffArray, $bothOffArray;

        $json = json_encode($washOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($dryOffArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($bothOffArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $fp = fopen('../data/machineStatus.txt', 'w');
        fwrite($fp, "*Washer ON\n");
        fclose($fp);
    }

    function setDryerOn() {

        global $washOffArray, $dryOnArray, $bothOffArray;

        $json = json_encode($washOffArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($dryOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($bothOffArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $fp = fopen('../data/machineStatus.txt', 'w');
        fwrite($fp, "*Dryer ON\n");
        fclose($fp);
    }

    function setBothOn() {

        global $washOnArray, $dryOnArray, $bothOnArray;

        $json = json_encode($washOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($dryOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $json = json_encode($bothOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $fp = fopen('../data/machineStatus.txt', 'w');
        fwrite($fp, "*Both ON\n");
        fclose($fp);
    }
    
    function startNotifier() {

        global $notifierOnArray;

        $json = json_encode($notifierOnArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $fp = fopen('../data/notifierStatus.txt', 'w');
        fwrite($fp, "*Notifier Set\n");
        fclose($fp);
    }

    function stopNotifier() {

        $json = json_encode($notifierOffArray);
        $fp = fopen('../data/xivelyFeed.json', 'w');
        fwrite($fp, stripslashes($json));
        fclose($fp);

        $output = shell_exec("./scripts/sendXivelyFeed.sh");
        //echo "<pre>$output</pre>";

        $fp = fopen('../data/notifierStatus.txt', 'w');
        fwrite($fp, "*Notifier Off\n");
        fclose($fp);
    }

    if ($washer == "ON" && $both == "NO") {

        setWasherOn();
    }

    if ($dryer == "ON" && $both == "NO") {

        setDryerOn();
    }
  
    if ($both == "YES") {

        setBothOn();
    }

   if ($notifier == "SET") {

        startNotifier();
   }

?>
