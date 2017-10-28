#!/bin/bash
# sublime text view->line ending-> unix to run 
docker-compose exec gcc echo 'CHECKING IF CONTAINER IS UP' # get id of docker container to check if one was up

# if id was null, start it up
if [ $? -eq 1 ]; then
	echo "DOCKER CONTAINER IS --NOT-- RUNNING "
	docker-compose up -d
else
	echo "DOCKER CONTAINER IS RUNNING "
fi

echo ""
echo "----------------------------------------------"
echo ""

# now read options from command line
while getopts mcr:hd:e option;do
	case $option in
	e)
	  docker-compose exec gcc bash
	  ;;
	h)
		echo "-m for make"
		echo "-r for run"
		echo "-r needs arguement"
		exit 1
		;;
    m)
	  echo "RUNNING MAKE on code in /code"
	  echo ""
	  docker-compose exec gcc bash -c "cd /code && make  $OPTARG"
	  exit 1
      ;;
    c)
		echo "CLEANING PROJECT"
		echo ""
	  docker-compose exec gcc bash -c "cd /code && make clean"
	  exit 1
	  ;;
    r)
      echo "RUNNING main with args" >&2
	  echo ""
      docker-compose exec gcc bash -c "cd /code && ./main $OPTARG"
	  exit 1

      ;;
    d)
		echo "GOING INTO GDB"
		echo ""
        docker-compose exec gcc bash -c "cd /code && make clean"
	  	docker-compose exec gcc bash -c "cd /code && make debug"
        docker-compose exec gcc bash -c "cd /code && gdb --args main $OPTARG"
	  exit 1

        ;;

    \?)
      echo "Invalid option: -$OPTARG" >&2
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1

  esac
done 