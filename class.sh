# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    class.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nlonka <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/13 11:22:19 by nlonka            #+#    #+#              #
#    Updated: 2023/09/05 18:32:07 by nlonka           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC=includes/
SRCS=srcs/
HEADER="$INC/$1.hpp"
FILE="$SRCS/$1.cpp"
UP_NAME=$(echo "$1_hpp" | tr '[a-z]' '[A-Z]')

mkdir -p $INC
mkdir -p $SRCS


if [ $# -eq 0 ]; then

	echo "Usage: bash clash.sh <ClassName>"
	exit 1;
fi

if ! grep -q "$1.cpp" Makefile; then
	gsed -i "s/SRCS =.*/& $1.cpp/g" Makefile;
fi

if ! ls $INC | grep -q "$1.hpp"; then

touch $HEADER
echo "#ifndef $UP_NAME" > $HEADER
echo "# define $UP_NAME" >> $HEADER
echo >> $HEADER
echo "class $1" >> $HEADER
echo "{" >> $HEADER
echo "	public:" >> $HEADER
echo "		$1();" >> $HEADER
echo "		$1(const $1 &src);" >> $HEADER
echo "		~$1();" >> $HEADER
echo >> $HEADER
echo "		$1 &operator=(const $1 &rhs);" >> $HEADER
echo >> $HEADER
echo "};" >> $HEADER
echo >> $HEADER
echo "#endif" >> $HEADER
fi

if ! ls $SRCS | grep -q "$1.cpp"; then

touch $FILE
echo "#include \"$1.hpp\"" > $FILE
echo >> $FILE
echo "$1::$1()" >> $FILE
echo "{" >> $FILE
echo "	std::cout << \"constructed [$1]\" << std::endl;" >> $FILE
echo "}" >> $FILE
echo >> $FILE
echo "$1::$1(const $1 &src)" >> $FILE
echo "{" >> $FILE
echo "	std::cout << \"copy constructed [$1]\" << std::endl;" >> $FILE
echo "	*this = src;" >> $FILE
echo "}" >> $FILE
echo >> $FILE
echo "$1	&$1::operator=(const $1 &rhs)" >> $FILE
echo "{" >> $FILE
echo "	return (*this);" >> $FILE
echo "}" >> $FILE
echo >> $FILE
echo "$1::~$1()" >> $FILE
echo "{" >> $FILE
echo "	std::cout << \"destructed [$1]\" << std::endl;" >> $FILE
echo "}" >> $FILE
fi
