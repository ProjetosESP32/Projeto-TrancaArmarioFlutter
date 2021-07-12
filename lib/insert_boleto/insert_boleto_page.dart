import 'package:armario_tranca_ifmt/insert_boleto/insert_boleto_controller.dart';
import 'package:armario_tranca_ifmt/themes/app_colors.dart';
import 'package:armario_tranca_ifmt/themes/app_text_styles.dart';
import 'package:armario_tranca_ifmt/widgets/input_text/input_text_widget.dart';
import 'package:armario_tranca_ifmt/widgets/set_label_buttons/set_label_buttons.dart';
import 'package:flutter/material.dart';
import 'package:font_awesome_flutter/font_awesome_flutter.dart';

class InsertInfos extends StatefulWidget {
  final String? barcode;
  const InsertInfos({
    Key? key,
    this.barcode,
  }) : super(key: key);

  @override
  _InsertInfosState createState() => _InsertInfosState();
}

class _InsertInfosState extends State<InsertInfos> {
  final controller = InsertBoletoController();

  final moneyInputTextController = TextEditingController();
  final vencimentoInputTextController = TextEditingController();
  final codigoInputTextController = TextEditingController();

  @override
  void initState() {
    if (widget.barcode != null) {
      codigoInputTextController.text = widget.barcode!;
    }
    super.initState();
  }

  void _showDialog() {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        // retorna um objeto do tipo Dialog
        return AlertDialog(
          title: new Text("Alert Dialog titulo"),
          content: new Text("Alert Dialog body"),
          actions: <Widget>[
            // define os botões na base do dialogo
            new CloseButton(
              color: AppColors.primary,
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: AppColors.background,
      appBar: AppBar(
        elevation: 0,
        backgroundColor: AppColors.background,
        leading: BackButton(
          color: AppColors.input,
        ),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.symmetric(horizontal: 24),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.center,
            children: [
              Padding(
                padding:
                    const EdgeInsets.symmetric(horizontal: 93, vertical: 24),
                child: Text(
                  "Preencha os dados da pessoa",
                  style: TextStyles.titleBoldHeading,
                  textAlign: TextAlign.center,
                ),
              ),
              Form(
                key: controller.formKey,
                child: Column(
                  children: [
                    InputTextWidget(
                      label: "Nome",
                      icon: Icons.people_alt,
                      onChanged: (value) {
                        controller.onChange(name: value);
                      },
                      validator: controller.validateName,
                    ),
                    InputTextWidget(
                      controller: vencimentoInputTextController,
                      label: "Senha",
                      icon: FontAwesomeIcons.key,
                      onChanged: (value) {
                        controller.onChange(senha: value);
                      },
                      validator: controller.validateVencimento,
                    ),
                    InputTextWidget(
                      controller: codigoInputTextController,
                      label: "validade",
                      icon: FontAwesomeIcons.calendar,
                      validator: controller.validateCodigo,
                      onChanged: (value) {
                        controller.onChange(validade: value);
                      },
                    )
                  ],
                ),
              )
            ],
          ),
        ),
      ),
      bottomNavigationBar: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Divider(
            height: 1,
            thickness: 1,
            color: AppColors.stroke,
          ),
          SetLabelButtons(
            enableSecondaryColor: true,
            labelPrimary: "Limpar",
            onTapPrimary: () {
              Navigator.pushReplacement(
                  context,
                  new MaterialPageRoute(
                      builder: (BuildContext context) => new InsertInfos()));
            },
            labelSecondary: "Cadastrar",
            onTapSecondary: () async {
              await controller.cadastrar();
            },
          ),
        ],
      ),
    );
  }
}
