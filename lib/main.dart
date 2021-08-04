//@dart=2.9
import 'package:flutter/material.dart';

import 'insert_pessoa/insert_pessoas_page.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.green,
      ),
      home: InsertInfos(),
    );
  }
}
