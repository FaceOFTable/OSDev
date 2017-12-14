<?php

include 'dither.class.php';

$dt = new Dithering();

$im = imagecreatefrompng($argv[1]);
$image = $dt->create($im);

imagepng($image, "result.png");
