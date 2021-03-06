node default {

  ## Defaults

  # Always run apt-get update before trying to install packages
  Package {
    require => Exec["aptitude_update"],
  }

  $db_name = "devel"
  $db_password = "pass"

  include "devel"
}

class devel {

  include "aptitude"
  include "vim"
  include "mysql"
  include "postgresql"
  include "java7"
  include "mongodb"
  include "python_extras"
  include "tmux"
}
