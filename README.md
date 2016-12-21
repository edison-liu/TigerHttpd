# TigerHttpd - A web server
==================================

This is a web server. The objective of this project is to implement a web server C++ on a UNIX-based platform.

# Features:
    - Based on pure C++11 without any external dependency.

# Roadmap
    Ø 0.1.0
        - Building directory.
        - Single thread. 
        - Get a static object
    Ø 0.2.0
        - Get a directory
            ○ Directory listing
            ○ HTTP 301 Redirect for directory without trailing slash.
        - Execute CGI programs    
    Ø 0.3.0
    Ø 0.4.0
    Ø 0.5.0
    Ø 0.6.0
    Ø 0.7.0
    Ø 0.8.0
    Ø 0.9.0
    Ø 1.0.0

# Build and Run

First, compile *TigerHttpd* from source,

```bash
$ cmake .
$ make
```

Now you can specify the **port number** and **ducument root directory** to run *TigerHttpd*,

```bash
$ ./tigerhttpd 0.0.0.0 8080 /home/eliu/TigerHttpd/tests/docroot/www
```
