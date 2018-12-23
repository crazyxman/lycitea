<?php

$lycilru = new Lycitea\Cache\lru('v1', 4);



$lycilru->set("hello1", "world");


$lycilru->set("hello2", "world");
$lycilru->set("hello3", "world");
$lycilru->set("hello4", "world");

$lycilru->set("hello5", "world");
$lycilru->set("hello6", "world");
$lycilru->set("hello3", "world");
$lycilru->set("hello4", "world");




$all = $lycilru->all();

print_r($all);



$lycilru->set("hello1", "world");


$lycilru->set("hello2", "world");
$lycilru->set("hello3", "world");
$lycilru->set("hello4", "world");

$lycilru->set("hello5", "world");
$lycilru->set("hello6", "world");
$lycilru->set("hello3", "world");
$lycilru->set("hello4", "world");




$all = $lycilru->all();

print_r($all);

$all = $lycilru->versions();

print_r($all);

