class java7 {

  package {
    "openjdk-7-jdk": ensure => installed;
    "openjdk-7-jre": ensure => installed;
    "maven": ensure => installed;
  }

  exec {
    "update-alternatives java":
    require => Package["openjdk-7-jdk"],
    command => "/usr/sbin/update-alternatives --set java /usr/lib/jvm/java-7-openjdk-amd64/jre/bin/java"
  }

  exec {
    "update-alternatives javac":
    require => Package["openjdk-7-jdk"],
    command => "/usr/sbin/update-alternatives --set javac /usr/lib/jvm/java-7-openjdk-amd64/bin/javac"
  }
}