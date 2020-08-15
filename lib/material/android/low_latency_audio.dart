import 'dart:ffi';

import 'package:ffi/ffi.dart';

_AudioManager _instance = _AudioManager._private();

class _AudioManager {
  final DynamicLibrary _lib = DynamicLibrary.open("liblow-latency-audio.so");

  void Function(Pointer handle) _releasePlayerJni;
  void Function(Pointer handle) _playPlayer;
  void Function(Pointer handle) _pausePlayer;

  void Function(Pointer handle, Pointer<Utf8> fileName) _loadAsset;

  Pointer Function() _createPlayer;

  _AudioManager._private() {
    _releasePlayerJni = _lib
        .lookup<NativeFunction<Void Function(Pointer handle)>>("releasePlayer")
        .asFunction();

    _loadAsset = _lib
        .lookup<
            NativeFunction<
                Void Function(
                    Pointer handle, Pointer<Utf8> fileName)>>("loadAsset")
        .asFunction();

    _createPlayer = _lib
        .lookup<NativeFunction<Pointer Function()>>("createPlayer")
        .asFunction<Pointer Function()>();

    _playPlayer = _lib
        .lookup<NativeFunction<Void Function(Pointer handle)>>("play")
        .asFunction();

    _pausePlayer = _lib
        .lookup<NativeFunction<Void Function(Pointer handle)>>("pause")
        .asFunction();
  }
}

class AudioPlayer {
  Pointer _handle;

  bool _isPlaying = false;

  AudioPlayer() {
    _handle = _instance._createPlayer();
  }

  void load(String path) {
    final utf8 = Utf8.toUtf8(path);
    _instance._loadAsset(_handle, utf8);
  }

  void play() {
    _instance._playPlayer(_handle);
    _isPlaying = true;
  }

  void pause() {
    if (_isPlaying) {
      _isPlaying = false;
      _instance._pausePlayer(_handle);
    }
  }

  void release() {
    _instance._releasePlayerJni(_handle);
  }
}
