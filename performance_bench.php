<?php
error_reporting(E_ALL);
require __DIR__ . '/FastRoute-master/src/bootstrap.php';

function callback() {}
/*$options = [
    'dataGenerator' => 'FastRoute\\OldDataGenerator',
    'dispatcher' => 'FastRoute\\OldDispatcher',
];*/
$options = [];
$nRoutes = 100;
$nMatches = 30000;

//var_dump($route);
$router = FastRoute\simpleDispatcher(function($router) use ($nRoutes, &$lastStr) {
    for ($i = 0, $str = 'a'; $i < $nRoutes; $i++, $str++) {
        $router->addRoute('GET', '/' . $str . '/{arg}', 'handler' . $i);
        $lastStr = $str;
    }
}, $options);
// first route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/a/foo');
}
printf("FastRoute first route: %f\n", microtime(true) - $startTime);
//var_dump($res);
// last route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/' . $lastStr . '/foo');
}
printf("FastRoute last route: %f\n", microtime(true) - $startTime);
//var_dump($res);
// unknown route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/foobar/bar');
}
printf("FastRoute unknown route: %f\n", microtime(true) - $startTime);



$lycilru = new Lycitea\Cache\lru('v1', 100);
for ($i = 0, $str = 'a'; $i < $nRoutes; $i++, $str++) {
    $lycilru->set('GET' . '/' . $str . '/{arg}', 'handler' . $i);
    $lastStr = $str;
}

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'.'/a/{arg}');
}
printf("Lycitea lru first route: %f\n", microtime(true) - $startTime);
//var_dump($res);

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'. '/' . $lastStr . '/{arg}');
}
printf("Lycitea lru last route: %f\n", microtime(true) - $startTime);
//var_dump($res);

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'. '/foobar/bar');
}
printf("Lycitea lru unknown route: %f\n", microtime(true) - $startTime);
//var_dump($res);

//var_dump($res);
echo "\n-----\n\n";
$nRoutes = 100;
$nArgs = 9;
$nMatches = 20000;
$args = implode('/', array_map(function($i) { return ':arg' . $i; }, range(1, $nArgs)));

//var_dump($route);
$args = implode('/', array_map(function($i) { return "{arg$i}"; }, range(1, $nArgs)));
$router = FastRoute\simpleDispatcher(function($router) use($nRoutes, $args, &$lastStr) {
    for ($i = 0, $str = 'a'; $i < $nRoutes; $i++, $str++) {
        $router->addRoute('GET', '/' . $str . '/' . $args, 'handler' . $i);
        $lastStr = $str;
    }
}, $options);
// first route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/a/' . $args);
}
printf("FastRoute first route: %f\n", microtime(true) - $startTime);
//var_dump($res);
// last route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/' . $lastStr . '/' . $args);
}
printf("FastRoute last route: %f\n", microtime(true) - $startTime);
//var_dump($res);
// unknown route
$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $router->dispatch('GET', '/foobar/' . $args);
}
printf("FastRoute unknown route: %f\n", microtime(true) - $startTime);
//var_dump($res);


$lycilru = new Lycitea\Cache\lru('v2', 100);

for ($i = 0, $str = 'a'; $i < $nRoutes; $i++, $str++) {
    $lycilru->set('GET'. '/' . $str . '/' . $args, 'handler' . $i);
    $lastStr = $str;
}

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'. '/a/' . $args);
}
printf("Lycitea lru first route: %f\n", microtime(true) - $startTime);
//var_dump($res);

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'. '/' . $lastStr . '/' . $args);
}
printf("Lycitea lru  last route: %f\n", microtime(true) - $startTime);
//var_dump($res);

$startTime = microtime(true);
for ($i = 0; $i < $nMatches; $i++) {
    $res = $lycilru->get('GET'. '/foobar/' . $args);
}
printf("FastRoute unknown route: %f\n", microtime(true) - $startTime);
//var_dump($res);