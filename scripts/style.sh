#!/bin/sh

# Apply kdelibs coding style to all c, cpp and header files in and below the current directory
#
# The coding style is defined in http://techbase.kde.org/Policies/Kdelibs_Coding_Style
#
# Requirements:
# - installed astyle, with patches below
#
# IMPORTANT: astyle misparses the Qt "keywords" like foreach and Q_FOREACH, which
# makes it style wrongly not just the space after the keyword, but also everything
# inside the parenthesis, leading to things like:
# Q_FOREACH(const QString & it, l) // note the space after the '&'.
#
# To fix this, patch astyle with http://www.davidfaure.fr/kde/astyle_qt.diff
# Mentionned upstream in https://sourceforge.net/p/astyle/bugs/154/
#
# Another feature was implemented: removing spaces before ',' and ';'.
# Apply http://www.davidfaure.fr/kde/astyle_comma.diff
# This has been sent upstream in https://sourceforge.net/p/astyle/bugs/100/
#
# Instructions for OpenSuSE users:
#   zypper si astyle
#   cd ~/rpmbuild/SOURCES ; wget http://www.davidfaure.fr/kde/astyle_qt.diff
#                           wget http://www.davidfaure.fr/kde/astyle_comma.diff
#   cd ../SPECS ; wget http://www.davidfaure.fr/kde/astyle.spec.diff
#   patch astyle.spec < astyle.spec.diff
#   rpmbuild -ba astyle.spec
#   sudo rpm -Uvh --force ~/rpmbuild/RPMS/x86_64/astyle-*.rpm

files=`find -type f -name '*.c' -or -name '*.cpp' -or -name '*.cc' -or -name '*.h'`
if [ -z "$files" ]; then
  # nothing to do
  exit 0
fi

astyle -n -Q \
      --indent=spaces=4 --style=otbs \
      --indent-labels --pad-oper --unpad-paren --pad-header \
      --keep-one-line-statements \
      --convert-tabs \
      --indent-preprocessor \
      --align-pointer=name \
      $files

# --max-code-length=100 is not used, on purpose. It's a hard limit, sometimes making things
# less readable than if the code is kept on a single line. 100 is just a recommendation
# in the coding style.

# Watch out for things that lead to method implementations being parsed as if inside other methods,
# e.g. due to '{' inside #ifdef and #else and '}' outside.
grep '^\S* \S*::.*) {$' $files && echo "WARNING: check for wrong '{' placement in method definitions, in above grep results"

# Remove old emacs mode-lines
perl -pi -e '$_ = "" if /c-basic-offset: [1-8]/' $files

# Remove old kate mode-lines
perl -pi -e '$_ = "" if /kate: .*indent-width/ || /kate:.*tab-width/' $files

# Remove old vim mode-lines
perl -pi -e '$_ = "" if /\/\/.* vim:/' $files
# They are often in a two-liner C comment, so we need a bit of perl magic to remove these
perl - $files <<EOF
  foreach my \$file (@ARGV) {
   open F, "+<", \$file or do { print STDERR "open(\$file) failed : \"\$!\"\n"; next };
   my \$str = join '', <F>;
   if( \$str =~ m/vim:/ ) {
     #print STDERR "Removing multi-line vim modeline from \$file\n";
     \$str =~ s!/\*\**\s*\**\s*vim:[^\n]*\n\s*\*/!!smg;
     seek F, 0, 0;
     print F \$str;
     truncate F, tell(F);
   }
   close F;
  }
EOF

# Remove consecutive blank lines
perl - $files <<EOF
  foreach my \$file (@ARGV) {
   open F, "+<", \$file or do { print STDERR "open(\$file) failed : \"\$!\"\n"; next };
   my \$str = join '', <F>;
   if (\$str =~ s/\s*\n\s*\n\s*\n\n*/\n\n/smg ) {
     seek F, 0, 0;
     print F \$str;
     truncate F, tell(F);
   }
   close F;
  }
EOF

# TODO: add command-line option to trigger this
# It's not wanted when working on a fix
#git commit -q -a -m "Code reformatted using kde-dev-scripts/astyle-kdelibs.
#Use git blame -w `git rev-parse --short HEAD` to show authorship as it was before this commit."

