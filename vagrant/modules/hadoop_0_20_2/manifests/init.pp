class hadoop_0_20_2 {
 $hadoop_home = "/opt/hadoop"

 exec { "download_hadoop":
 command => "wget -O /tmp/hadoop.tar.gz http://archive.apache.org/dist/hadoop/core/hadoop-0.20.2/hadoop-0.20.2.tar.gz",
 path => $path,
 unless => "ls /opt | grep hadoop-0.20.2",
 require => Package["openjdk-6-jdk"]
}

 exec { "unpack_hadoop" :
   command => "tar -zxf /tmp/hadoop.tar.gz -C /opt",
   path => $path,
   creates => "${hadoop_home}-0.20.2",
   require => Exec["download_hadoop"]
 }

file {
  "${hadoop_home}-0.20.2/conf/slaves":
  source => "puppet:///modules/hadoop/slaves",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }

file {
  "${hadoop_home}-0.20.2/conf/masters":
  source => "puppet:///modules/hadoop/masters",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }

file {
  "${hadoop_home}-0.20.2/conf/core-site.xml":
  source => "puppet:///modules/hadoop/core-site.xml",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }
 
file {
  "${hadoop_home}-0.20.2/conf/mapred-site.xml":
  source => "puppet:///modules/hadoop/mapred-site.xml",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }
 
 file {
  "${hadoop_home}-0.20.2/conf/hdfs-site.xml":
  source => "puppet:///modules/hadoop/hdfs-site.xml",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }

file {
  "${hadoop_home}-0.20.2/conf/hadoop-env.sh":
  source => "puppet:///modules/hadoop/hadoop-env.sh",
  mode => 644,
  owner => root,
  group => root,
  require => Exec["unpack_hadoop"]
 }

}