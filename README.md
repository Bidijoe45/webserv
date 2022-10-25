# webserv

This is an HTTP web server written in C++.

## Usage

1. clone repo
2. `make && ./webserv [settings_file_path]` (if no settings file is specified, the default `server.conf` will be used)

## Server configuration file

**Directives:**

* `server` => block directive delimiting each server's settings.
* `listen` => the server will listen on this port. Must be a number.
* `server_name` => the name of the server.
* `client_max_body_size` => limits the maximum size of the request body, in bytes. Must be a number.
* `error_page` => allows you to set a custom error page for each response status code. The format is `status_code path_to_page`.
* `location` => block directive delimiting each location's settings. These are configurations for processing URIs beginning with the specified location path.
    * `root` => replaces the location path with the root path.
    * `index` => default file to return if the request target is a directory.
    * `autoindex` => can be `on` or `off`. If it is `on`, and there is no `index` directive, this will list the contents of the requested directory.
    * `accept` => allowed methods for this location.
    * `redirect` => the format is `status_code new_location`. A request to this location will redirect to the location specified by `new_location`. 
    * `cgi` => the format is `file_extension absolute_path_to_cgi_executable`. If this directive is present, all files ending with `file_extension` will be handled by the cgi programme specified in `path_to_cgi_executable`.

**Settings file example:**

     server {
     
        listen 80;
        server_name localhost;
        error_page 404 ./www/404_custom.html;
        location / {
            root ./www/;
            accept GET POST DELETE;
            upload ./www/upload/;
            index index.html;
        }
        
        location /formula1 {
            root ./www/formula1;
            accept GET POST DELETE;
            autoindex on;
            cgi .php /Users/xxx/webserv/cgi-bin/php-cgi;
            cgi .py /usr/bin/python3;
        }
        
        location /coche {
            accept GET;
            redirect 301 /formula1/;
        }
    }

