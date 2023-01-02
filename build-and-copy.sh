#!/usr/bin/env bash

pushd contracts/marble/build
make
popd
cp ./contracts/marble/build/marble/marble.abi ../BennyFi/artifacts/marble.abi
cp ./contracts/marble/build/marble/marble.wasm ../BennyFi/artifacts/marble.wasm