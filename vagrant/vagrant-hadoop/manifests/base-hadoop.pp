include hadoop

group { "puppet":
  ensure => "present",
}

exec { 'apt-get update':
    command => '/usr/bin/apt-get update',
}

package { "openjdk-6-jdk" :
   ensure => present,
  require => Exec['apt-get update']
}

package { "emacs" :
   ensure => present,
  require => Exec['apt-get update']
}

file { "/root/.ssh":
    ensure => "directory",
    owner  => "root",
    group  => "root",
    mode   => 750,
    require => Exec['apt-get update']
}

file {
  "/root/.ssh/id_rsa":
  source => "puppet:///modules/hadoop/hadoop_id_rsa",
  mode => 600,
  owner => root,
  group => root,
  require => File['/root/.ssh']
 }
 
file {
  "/root/.ssh/id_rsa.pub":
  source => "puppet:///modules/hadoop/hadoop_id_rsa.pub",
  mode => 644,
  owner => root,
  group => root,
  require => File['/root/.ssh']
 }

ssh_authorized_key { "ssh_key":
    ensure => "present",
    key    => "AAAAB3NzaC1yc2EAAAADAQABAAABAQCscCkzaLA4ilmjTBmfkEBH4s44w37CISLAa7qCB8CAxWG9Dh/uZ0IkrGU8OulJlXO4ufxHt2oTKsmgPwpUxTeU2/jAWOWofPxmi/KVTVMLEQ5yOYd4YSafVWeyYV0UrLG0t9HUkXiR/lzQD1urhs4hSpPrr1ICGsc6JhlO0Oumdp6yBvcrDBWx7AhZcIpJtp0/uAe2tAXFBvS3Jls5tPDPYRHEmjT/ZpuObSGPRJUxnM4/DuVr1A3zCKhewV9d3gJYC36Eb5ZGyU5cV3NL/Vcr/Vx3bb4xKP/C4wgh/nIkVzire0MgpdXBG7UIozWqu4Tl8gWSr8uoXGTgaLG0PfvR",
    type   => "ssh-rsa",
    user   => "root",
    require => File['/root/.ssh/id_rsa.pub']
}