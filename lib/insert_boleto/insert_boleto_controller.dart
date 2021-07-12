import 'package:armario_tranca_ifmt/insert_boleto/database.dart';
import 'package:armario_tranca_ifmt/models/boleto_model.dart';
import 'package:http/http.dart' as http;
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
    final instance = await SharedPreferences.getInstance();
    final boletos = instance.getStringList("boletos") ?? <String>[];
    boletos.add(model.toJson());
    await instance.setStringList("boletos", boletos);
    print("Entrou aqui");

    return;
  }

  Future<void> cadastrar() async {
    final form = formKey.currentState;
    if (form!.validate()) {
      return await saveBoleto();
    }
  }

  /*Future<void> go(BoletoModel BoletoModel) async {
    var response =
        await http.put(Uri.parse("https://oie1-a40fc.firebaseio.com"), body: {
      'Proprietario': BoletoModel.name,
      'SenhaGerada': BoletoModel.senha,
      'Validade': BoletoModel.validade,
    });
    print('Response status: ${response.statusCode}');
    print('Response body: ${response.body}');
    print("Database");
  }*/
}
