node default {

  ## Defaults

  # Always run apt-get update before trying to install packages
  Package {
    require => Exec["aptitude_update"],
  }

  $database_name = "devel"

  $mysql_password = "mysql"

  include "devel"
}

class devel {

  include "aptitude"
  include "emacs"
  include "mysql"
  include "mongodb"
}
