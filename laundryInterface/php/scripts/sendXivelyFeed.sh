cd ../data
curl --request PUT \
     --data-binary @xivelyFeed.json \
     --header "X-ApiKey: put your Xively Api key here" \
     --verbose \
     https://api.xively.com/v2/feeds/put your Xively Feed ID here