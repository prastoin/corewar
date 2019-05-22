#!/usr/bin/env nix-shell
#!nix-shell -i bash -p curl

version=$(git rev-parse --short HEAD)

curl -T vm.wasm -H "Content-Type: application/wasm" -udeltaevo:$BINTRAY_KEY https://api.bintray.com/content/deltaevo/corewar/vm/$version/$version/vm.wasm
curl -X POST -udeltaevo:$BINTRAY_KEY https://api.bintray.com/content/deltaevo/corewar/vm/$version/publish
sleep 20
curl -X PUT -H "Content-Type: application/json" -d '{"list_in_downloads": true}' -udeltaevo:$BINTRAY_KEY \
	https://api.bintray.com/file_metadata/deltaevo/corewar/$version/vm.wasm
