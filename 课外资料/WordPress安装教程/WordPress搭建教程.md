# WordPress搭建教程（基于CentOS 7）

## 前言

​    wordpress也装了很多次了，每次总会遇到大大小小的问题，百度上的答案也看的我眼花缭乱，借着这次机会，简单记录下安装的过程，以及安装过程中遇到的一些问题的解决方法。

## 相关基础知识

### 静态网站和动态网站的区别

- 静态网站：指全部由HTML代码格式页面组成的网站，所有的内容包含在网页文件中。

  主要用到的技术：

  - HTML标记语言
  - CSS样式表
  - JavaScript脚本语言

  特点：

  - 服务器负担小；
  - 内容相对稳定，有利于搜索引擎优化SEO，Baidu、Google都会优先收录静态页面，不仅被收录的快还收录的全；
  - 页面打开速度快，无需连接数据库；
  - 静态网站没有数据库的支持，在网站制作和维护方面工作量较大。

- 动态网站：除了要设计网页以外，还要通过数据库和编程序来进行架构的网站。

  主要用到的技术

  - HTML标记语言
  - CSS样式表
  - JavaScript脚本语言
  - 服务器端脚本语言
  - 数据库

  特点：

  - 以数据库技术为基础，网站维护的工作量小；
  - 可以实现更多的动能，如用户注册、用户登录、在线调查、用户管理、订单管理等；
  - 动态网页实际上并不是独立存在于服务器上的网页文件，只有当用户请求时服务器才返回一个完整的网页。

### 搭建动态网站需要的内容包括

1. 操作系统
   - Windows
   - Linux
2. Web服务器
   - IIS
   - Apache
   - Nginx
3. 数据库工具
   - SQL Server
   - MySQL
   - Oracle
4. 开发语言
   - ASP
   - JSP
   - PHP

### 动态网站的常见实现方式

1. Windows + IIS + SQL Server + ASP
2. Linux + Apache + MySQL + PHP（LAMP）
3. Linux + Nginx + MySQL + PHP（LNMP）

### Linux常用命令

1. yum -y install <软件包>从云端安装软件包，对所有的提问都默认为y
2. systemctl start/stop/restart <某服务.service>  启动/停止/重启该服务（CentOS 7专属启动服务命令）
3. systemctl enable <某服务.service> 设置该服务开机自启

## 基本步骤（LAMP）

### 创建云服务器

1. 买服务器这些就不用说了吧，我是在阿里云买的学生服务器一年120，据说腾讯云也有学生服务器价格也差不多；

2. 需要注意的几点，购买服务器时镜像选择Linux的镜像，在这里我选择的是CentOS 7.4；

3. 设置服务器密码；

4. 设置安全组规则，阿里云如下

   - 经典网络

     公网入方向，放行端口：

     - TCP 22：SSH 远程连接 Linux 实例；
     - TCP 80：实例对外提供 Web 服务；
     - TCP 21/22：使用 FTP 服务；
     - TCP 9000：如果开始了 PHP 进程服务，需要放行这个端口。

   - 专有网络

     入方向，放行端口：

     - TCP 22：SSH 远程连接 Linux 实例；
     - TCP 80：实例对外提供 Web 服务；
     - TCP 21/22：使用 FTP 服务；
     - TCP 9000：如果开始了 PHP 进程服务，需要放行这个端口。

5. 安装工具Xftp/Xshell

6. [官网下载WordPress安装包](https://wordpress.org/download/)。

### 安装和配置开发环境

1. 使用Xshell连接服务器；

2. 安装Apache及拓展，并设置启动

   ```shell
   yum -y install httpd #安装Apache，提示Complete! 即安装成功
   yum -y install httpd-manual mod_ssl mod_perl #安装Apache拓展，提示Complete! 即安装成功
   systemctl start httpd.service #启动Apache服务，此时可以测试Apache是否安装成功。即在浏览器中输入域名或IP访问，会出现Testing 123... 界面
   systemctl enable httpd.service #设置开机自启动
   ```
   ​

3. 安装PHP

   ```shell
   yum -y install php #安装PHP，提示Complete! 即安装成功
   yum -y install gd php-fpm php-gd php-devel php-xml php-common php-mbstring php-ldap php-pear php-xmlrpc php-imap php-cli #安装PHP拓展，提示Complete! 即安装成功
   systemctl restart httpd.service #重启Apache服务
   #测试PHP安装是否成功方法：
   echo "<?php phpinfo(); ?>" > /var/www/html/phpinfo.php
   #输入域名或IP/phpinfo.php，会出现PHP信息界面
   ```

4. 安装MySQL（CentOS中Mariadb代替了MySQL）

   ```shell
   yum -y install mariadb mariadb-server #安装数据库Maria DB
   yum -y install php-mysql
   yum -y install mysql-connector-odbc libdbi-dbd-mysql mysql-devel #安装Maria DB拓展
   systemctl start mariadb.service #启动数据库服务
   systemctl enable mariadb.service #设置开机启动
   #配置MariaDB安全性
   mysql_secure_installation #进入安全性配置界面
   #详细配置见下文
   #配置完成后，重启所有安装的服务
   #登录MariaDB
   mysql -uroot -p
   #提示 Enter password:  输入密码时不显示字符
   #登录成功后提示
   MariaDB[(none)]>
   #输入命令：
   create database wordpress; #新建数据库来做wordpress的数据库，必须加分号！
   #输入quit退出：
   quit
   systemctl restart httpd.service #重启Apache服务
   systemctl restart mariadb.service #重启MariaDB服务
   ```

   配置MariaDB

   进入MariaDB配置后，依次提示：

   - 输入当前密码(初次为空,直接回车即可) （按回车）
   - 是否设置密码？（y）
   - 输入新密码（输入新密码）
   - 确认密码（再次输入密码）
   - 是否删除匿名账户？（y）
   - 受否不允许远程连接？（n）
   - 是否删除测试库？（y）
   - 重新载入权限表？（y）

### 部署WordPress网站

1. 使用Xftp连接服务器；

2. 将wordpress压缩包解压，并上传至/var/www/html/中，注意若上传一整个文件夹，则wordpress是通过域名或ip/wordpress进行访问，若想一打开网站就访问wordpress，则需将wordpress里的文件直接上传至/var/www/html/；

3. *暂时将wordpress目录设置权限为777，防止无法配置wp-config,php；*

4. 浏览器访问ip或域名/wordpress（安装目录），输入相关信息进行配置；

5. 配置完成后，将wordpress目录权限改回755，同时在ftp中修改wordpress目录下的wp-config.php文件，添加一行代码，[防止无法ftp连接服务器](https://www.zhihu.com/question/28910605/answer/50886745) ；

   ```php
   define('FS_METHOD', "direct");
   ```

6. 修改wordpress权限，以[解决无法上传文件/主题等错误](https://my.oschina.net/kjpioo/blog/162697)

   ```shell
   # 首先需要你有root权限，SSH登录，进入到wp的安装目录： 
   cd /var/www/html/wordpress
   # 给予所有的写权限： 
   chmod 777 wp-content 
   # 接下来给你的博客的文章上传一张图片，WP会生成一个目录， 然后查看是哪个用户创建了文件夹。一般情况下，这个用户名叫“ apache”，也有不少人发现这个用户是“nobody”，具体视情况而定;
   # 进入到wp的wp-content目录，查看该目录下所有文件/ 文件夹的权限，所属用户、用户组： 
   cd wp-content
   ls -l 
   # 查看最后一条命令的创建用户一般为www或apache
   # 上传目录 uploads 是用户 apache 创建的
   # 接下来把wp-content权限还原到 755：
   cd ..
   chmod 755 wp-content 
   # 接下来就是实际修复的命令了， 改变wp所在文件夹的拥有者为刚找到的这个用户www： 
   cd ..
   chown -R apache:apache wordpress
   # OK，问题解决。
   ```

7. 至此，你已经完成了wordpress的安装与配置，剩下的主题配置等就看自己喜好啦~ 

## 参考资料及感谢

- [阿里云搭建WordPress网站帮助文档](https://help.aliyun.com/document_detail/43244.html?spm=a2c4g.11186623.6.747.BTCKNC)
- [阿里云网站建设教程（需购买）](https://market.aliyun.com/products/57252001/cmgj020213.html)
- [linux 安装LAMP环境(centos7.4)](http://blog.csdn.net/qiaosym/article/details/78710305)
- [Linux 下 MySQL(Mariadb) 安装与使用](https://my.oschina.net/u/260165/blog/1622181)
- [解决无法连接ftp](https://www.zhihu.com/question/28910605/answer/50886745)
- [解决无法上传文件/主题的错误](https://my.oschina.net/kjpioo/blog/162697)



