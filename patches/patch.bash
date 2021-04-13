VERSION=0.1
if [ $# -eq 0 -o $# -gt 1 ]
then
  echo ""
  echo "patch.bash version $VERSION"
  echo "It's an universal patch script that runs using Unix-like OS and Windows (with git-scm or WSL)"
  echo "It uses GNU patch distributed with Unix-like OS or bash provided by git-scm and WSL"
  echo "For Windows it requires git-scm <https://git-scm.com> or Windows WSL installed"
  echo "Usage: "
  echo "           \$ bash $0 \"<patch parameters>\""
  echo "Important: \"<patch parameters>\" $(tput bold)MUST$(tput sgr0) have double quotes."
  echo "           Without double quotes Windows can't interprept Windows-style PATH"
  echo "Usage example: "
  echo "           \$ bash $0 \"'c:\Users\User 1\file.cpp.diff' c:\devel\prog1\file.cpp\""
  echo "For more information about patch parametes: "
  echo "           \$ man patch # or <https://man7.org/linux/man-pages/man1/patch.1.html>"
  echo ""
  exit 1
fi

parameters=$1 # parametes
uname="$(uname -a)"
os="unix-like"

# check if git bash or other MINGW bash is being used
if [[ $uname == *"MINGW"* ]]; then
  os="mingw"
  parameters=$(echo "$parameters" | sed -e "s/\\\/\//g" | sed -r "s/([ |\"|\'])(.):\//\1\/\l\2\//" | sed -r "s/^(.):\//\/\l\1\//")
# check if WSL 1 or 2 is being used
elif [[ $uname == *"Microsoft"* ]]; then
  os="wsl"
  parameters=$(echo "$parameters" | sed -e "s/\\\/\//g" | sed -r "s/([ |\"|\'])(.):\//\1\/mnt\/\l\2\//" | sed -r "s/^(.):\//\/mnt\/\l\1\//")
fi

# Apply patch
echo $(patch $parameters)
