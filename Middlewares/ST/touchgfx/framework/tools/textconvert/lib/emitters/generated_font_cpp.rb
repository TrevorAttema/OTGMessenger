##############################################################################
# This file is part of the TouchGFX 4.16.1 distribution.
#
# <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
# All rights reserved.</center></h2>
#
# This software component is licensed by ST under Ultimate Liberty license
# SLA0044, the "License"; You may not use this file except in compliance with
# the License. You may obtain a copy of the License at:
#                             www.st.com/SLA0044
#
##############################################################################

class GeneratedFontCpp < Template
  def input_path
    File.join(root_dir,'Templates','GeneratedFont.cpp.temp')
  end
  def output_path
    '/src/GeneratedFont.cpp'
  end
  def output_filename
    File.join(@output_directory, output_path)
  end
  def run
    if (!File::exists?(output_filename))
      #generate GeneratedFont.cpp
      super
    end
  end
end
