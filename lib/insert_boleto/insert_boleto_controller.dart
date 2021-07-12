import 'package:armario_tranca_ifmt/models/boleto_model.dart';
import 'package:armario_tranca_ifmt/themes/app_colors.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';

import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

class InsertBoletoController {
  final formKey = GlobalKey<FormState>();
  BoletoModel model = BoletoModel();

  String? validateName(String? value) =>
      value?.isEmpty ?? true ? "O nome não pode ser vazio" : null;
  String? validateVencimento(String? value) =>
      value?.isEmpty ?? true ? "A senha não pode ser vazia" : null;
  String? validateCodigo(String? value) =>
      value?.isEmpty ?? true ? "A data de vencimento não pode ser vazio" : null;

  void onChange({String? name, String? senha, String? validade}) {
    model = model.copyWith(name: name, senha: senha, validade: validade);
  }

  Future<void> saveBoleto() async {
    WidgetsFlutterBinding.ensureInitialized();
    await Firebase.initializeApp();
    final fb = FirebaseDatabase.instance;
    final ref = fb.reference();
    print(ref.child("ARMARIO/Proprietario").set(model.name));
    print(ref.child("ARMARIO/Validade").set(model.validade));
    print(ref.child("ARMARIO/SenhaGerada").set(model.senha));

    return;
  }

  Future<void> cadastrar() async {
    final form = formKey.currentState;
    if (form!.validate()) {
      return await saveBoleto();
    }
  }
}
