import 'dart:convert';

class BoletoModel {
  final String? name;
  final String? senha;
  final String? validade;

  BoletoModel({
    this.name,
    this.senha,
    this.validade,
  });

  BoletoModel copyWith({
    String? name,
    String? senha,
    String? validade,
  }) {
    return BoletoModel(
      name: name ?? this.name,
      senha: senha ?? this.senha,
      validade: validade ?? this.validade,
    );
  }

  Map<String, dynamic> toMap() {
    return {
      'name': name,
      'senha': senha,
      'validade': validade,
    };
  }

  factory BoletoModel.fromMap(Map<String, dynamic> map) {
    return BoletoModel(
      name: map['name'],
      senha: map['senha'],
      validade: map['validade'],
    );
  }

  String toJson() => json.encode(toMap());

  factory BoletoModel.fromJson(String source) =>
      BoletoModel.fromMap(json.decode(source));

  @override
  String toString() {
    return 'BoletoModel(name: $name, senha: $senha, validade: $validade,)';
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;

    return other is BoletoModel &&
        other.name == name &&
        other.senha == senha &&
        other.validade == validade;
  }

  @override
  int get hashCode {
    return name.hashCode ^ senha.hashCode ^ validade.hashCode;
  }
}
