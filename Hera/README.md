# Hera
Log converter

| Option        | Explanation                                                |
| ------------- | ---------------------------------------------------------- |
| -i=lr         | get data from log files *(\*.gz/\*.log)* with logreader    |
| --if=filename | read log file **filename** when using ***-i=lr***          |
| --id=path     | read all log files in **PATH** when using ***-i=lr***      |
| -i=nr         | get data from UDP                                          |
| --ic          | log cutting with breakpoint imformation in **logclip.ini** |
| -o=lw         | store data to files with logwriter                         |
| --of=filename | store data to file **filename** (Only if ***-i=nr***)      |
| --od=path     | store data to file in **PATH** when using ***-o=lw***      |
| -o=ns         | send data from UDP                                         |
| -p=number     | set vision port to **number**                              |

