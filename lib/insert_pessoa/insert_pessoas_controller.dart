import 'package:armario_tranca_ifmt/models/pessoas_model.dart';
import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';

class InsertPessoasController {
  final formKey = GlobalKey<FormState>();
  PessoaModel model = PessoaModel();

  String? validateName(String? value) =>
      value?.isEmpty ?? true ? "O nome não pode ser vazio" : null;
  String? validateValidade(String? value) =>
      value?.isEmpty ?? true ? "A data de vencimento não pode ser vazio" : null;
  String? validateSenha(String? value) =>
      value?.isEmpty ?? true ? "A senha não pode ser vazia" : null;

  void onChange({String? name, String? senha, String? validade}) {
    model = model.copyWith(name: name, senha: senha, validade: validade);
  }

  Future<void> savePessoa() async {
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
      return await savePessoa();
    } else {
      throw ("err");
    }
  }
}
