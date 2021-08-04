import 'dart:convert';

class PessoaModel {
  final String? name;
  final String? senha;
  final String? validade;

  PessoaModel({
    this.name,
    this.senha,
    this.validade,
  });

  PessoaModel copyWith({
    String? name,
    String? senha,
    String? validade,
  }) {
    return PessoaModel(
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

  factory PessoaModel.fromMap(Map<String, dynamic> map) {
    return PessoaModel(
      name: map['name'],
      senha: map['senha'],
      validade: map['validade'],
    );
  }

  String toJson() => json.encode(toMap());

  factory PessoaModel.fromJson(String source) =>
      PessoaModel.fromMap(json.decode(source));

  @override
  String toString() {
    return 'PessoaModel(name: $name, senha: $senha, validade: $validade,)';
  }

  @override
  bool operator ==(Object other) {
    if (identical(this, other)) return true;

    return other is PessoaModel &&
        other.name == name &&
        other.senha == senha &&
        other.validade == validade;
  }

  @override
  int get hashCode {
    return name.hashCode ^ senha.hashCode ^ validade.hashCode;
  }
}
