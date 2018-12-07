# Lycitea - Fast Component collection 

Fast Component collection written in c, built in php 

### Requirement
* PHP 7.0 +

### Compile Yaf in Linux
```
$/path/to/phpize
$./configure --with-php-config=/path/to/php-config
$make && make install
```
### Performance
Dynamic route is faster than <a href="https://github.com/nikic/FastRoute" target="_blank">FastRoute</a> by from 73 times  to 92 times. Moreover static route performes better than Dynamic route.

### Usage
Here's a basic usage example:

```
$lyciroute = new Lycitea\Route\Simple();
$lyciroute->get('/cd', 'handle');

// Matches /cd/dir/php, but not /cd/dir/PHP
$lyciroute->get('/cd/dir/{name:[a-z]+}', 'handle', LYCITEA_ROUTE_SIMPLE_REGEX);

// Matches /cd/dirv/php/bin, but not /cd/dirv/bin
$lyciroute->get('/cd/dirv/{name:[a-z]+}[/bin]', 'handle', LYCITEA_ROUTE_SIMPLE_REGEX);

// Matches /cd/dirs/php/123, but not /cd/dirs/php/abc
$lyciroute->get('/cd/dirs/{name:[a-z]+}[/{dir:[0-9]}]', 'handle', LYCITEA_ROUTE_SIMPLE_REGEX);

$lyciroute->addRoute('POST', '/cd', 'handle');
$lyciroute->addRoute(['PUT', 'DELETE'], '/cd/dir/{name:[a-z]+}', 'handle', LYCITEA_ROUTE_SIMPLE_REGEX);

$lyciroute->addGroup('/h5', function($lyciroute){
    $lyciroute->get('/cd/ls', 'handle');
    $lyciroute->addRoute('POST', '/cd/ls', 'handle');
    $lyciroute->addGroup('/games', function($lyciroute){
        $lyciroute->get('/cd/ls', 'handle');
        $lyciroute->addRoute(['PUT', 'DELETE'], '/cd/ls', 'handle');
    }
}

$routeInfo = $lyciroute->dispatch($httpMethod, $uri);
switch ($routeInfo[0]) {
    case 0:
        // ... 404 Not Found
        break;
    case 1:
        $handler = $routeInfo[1];
        $vars = $routeInfo[2];
        // ... call $handler with $vars
        break;
}

```

