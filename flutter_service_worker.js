'use strict';
const CACHE_NAME = 'flutter-app-cache';
const RESOURCES = {
  "/index.html": "69d4fe2fd27d4e00761237c3a2995bde",
"/main.dart.js": "a97b4b04633940845025b5a6a32d7d3d",
"/icons/Icon-192.png": "ac9a721a12bbc803b44f645561ecb1e1",
"/icons/Icon-512.png": "96e752610906ba2a93c65f8abe1645f1",
"/manifest.json": "c06ff9ee8c7976e5bc1ac68e26f72884",
"/assets/LICENSE": "530b20cc4e1f9ffe4478a7bdb15c7e66",
"/assets/AssetManifest.json": "532f81b1a8bac6efd118107ceb4df33c",
"/assets/FontManifest.json": "580ff1a5d08679ded8fcf5c6848cece7",
"/assets/fonts/MaterialIcons-Regular.ttf": "56d3ffdef7a25659eab6a68a3fbfaf16",
"/assets/assets/material.png": "6daf304356cc010c1d23585c44041143",
"/assets/assets/audios/start.mp3": "a5ca4c3bbeeb2d74b90ec1554a55cb1b",
"/assets/assets/audios/explosion.mp3": "2c8b5dd3f51298bbeb019ce1316aa6b9",
"/assets/assets/audios/drop.mp3": "154540aeac037695ec1d80adfdff8936",
"/assets/assets/audios/clean.mp3": "ac70daf98741de473dec0eacdac333e8",
"/assets/assets/audios/rotate.mp3": "23789f16afc6b9cd22711ffde1baa57f",
"/assets/assets/audios/move.mp3": "389fd99cd348d712668fa34fd9738753",
"/assets/assets/alipay.jpg": "2b9c6010ae04652649b978fe7a8d35c4",
"/assets/assets/wechat.png": "199febd121794aabdee55fb9d0aeec92"
};

self.addEventListener('activate', function (event) {
  event.waitUntil(
    caches.keys().then(function (cacheName) {
      return caches.delete(cacheName);
    }).then(function (_) {
      return caches.open(CACHE_NAME);
    }).then(function (cache) {
      return cache.addAll(Object.keys(RESOURCES));
    })
  );
});

self.addEventListener('fetch', function (event) {
  event.respondWith(
    caches.match(event.request)
      .then(function (response) {
        if (response) {
          return response;
        }
        return fetch(event.request);
      })
  );
});
