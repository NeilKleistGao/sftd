cmake -DBUILD_TEST=OFF .
make

cmake -DBUILD_TEST=ON .
make

path="./bin/test"
echo "path: ${path}"
files=$(ls $path)
for file in ${files}
do
  res=$(exec "${path}/${file}" | grep "Failure")
  if [[ "$res" != "" ]]
  then
    exit 1
  fi
done
