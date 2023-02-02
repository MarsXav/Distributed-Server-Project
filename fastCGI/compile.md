If cannot find a dynamic library  when compailing following procedures are suggested:

```sh
sudo find / -name "[name_of_lib].so"
```
If found, a directory will be returned
Then add the directory to /etc/ld.so.conf

Then reapply the configuration file
```sh
sudo ldconfig
```

