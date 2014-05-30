class postgresql {

  package {
    "postgresql": ensure => installed;
    "python-psycopg2": ensure => installed;
  }

  file {
    "/usr/local/sbin/init_postgresql":
      content => template("postgresql/init_postgresql.erb"),
      mode => 0555;
  }

  exec {
    "init_postgresql":
      command => "/usr/local/sbin/init_postgresql",
      user => "postgres",
      require => [
        File["/usr/local/sbin/init_postgresql"],
        Package["postgresql"]
      ];
  }
}