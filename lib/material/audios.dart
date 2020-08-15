import 'dart:async';
import 'dart:io';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:soundpool/soundpool.dart';
import 'package:tetris/material/android/low_latency_audio.dart';

class Sound extends StatefulWidget {
  final Widget child;

  const Sound({Key key, this.child}) : super(key: key);

  @override
  SoundState createState() {
    if (Platform.isAndroid) {
      return _AndroidSoundState();
    } else {
      return _SoundState();
    }
  }

  static SoundState of(BuildContext context) {
    final state = context.findAncestorStateOfType<SoundState>();
    assert(state != null, 'can not find Sound widget');
    return state;
  }
}

const _SOUNDS = [
  'clean.mp3',
  'drop.mp3',
  'explosion.mp3',
  'move.mp3',
  'rotate.mp3',
  'start.mp3'
];

abstract class SoundState extends State<Sound> {
  bool mute = false;

  void start();

  void clear();

  void fall();

  void rotate();

  void move();

  @override
  Widget build(BuildContext context) {
    return widget.child;
  }
}

class _AndroidSoundState extends SoundState {
  Map<String, AudioPlayer> players = {};

  @override
  void initState() {
    super.initState();
    for (var value in _SOUNDS) {
      scheduleMicrotask(() async {
        final player = AudioPlayer();
        player.load('flutter_assets/assets/audios/$value');
        players[value] = player;
      });
    }
  }

  void _play(String name) {
    final player = players[name];
    players.forEach((key, value) {
      value.pause();
    });
    if (player != null && !mute) {
      player.play();
    }
  }

  @override
  void start() {
    _play('start.mp3');
  }

  @override
  void clear() {
    _play('clean.mp3');
  }

  @override
  void fall() {
    _play('drop.mp3');
  }

  @override
  void rotate() {
    _play('rotate.mp3');
  }

  @override
  void move() {
    _play('move.mp3');
  }
}

class _SoundState extends SoundState {
  Soundpool _pool;

  Map<String, int> _soundIds;

  void _play(String name) {
    final soundId = _soundIds[name];
    if (soundId != null && !mute) {
      _pool.play(soundId);
    }
  }

  @override
  void initState() {
    super.initState();
    _pool = Soundpool(streamType: StreamType.music, maxStreams: 4);
    _soundIds = Map();
    for (var value in _SOUNDS) {
      scheduleMicrotask(() async {
        final data = await rootBundle.load('assets/audios/$value');
        _soundIds[value] = await _pool.load(data);
      });
    }
  }

  @override
  void dispose() {
    super.dispose();
    _pool.dispose();
  }

  @override
  void start() {
    _play('start.mp3');
  }

  @override
  void clear() {
    _play('clean.mp3');
  }

  @override
  void fall() {
    _play('drop.mp3');
  }

  @override
  void rotate() {
    _play('rotate.mp3');
  }

  @override
  void move() {
    _play('move.mp3');
  }
}
