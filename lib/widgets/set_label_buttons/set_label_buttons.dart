import 'package:armario_tranca_ifmt/themes/app_colors.dart';
import 'package:armario_tranca_ifmt/widgets/label_button.dart/label_button.dart';
import 'package:flutter/material.dart';

class SetLabelButtons extends StatelessWidget {
  final String labelPrimary;
  final VoidCallback onTapPrimary;
  final String labelSecondary;
  final VoidCallback onTapSecondary;
  final bool enablePrimaryColor;
  final bool enableSecondaryColor;
  const SetLabelButtons({
    Key? key,
    required this.labelPrimary,
    required this.onTapPrimary,
    required this.labelSecondary,
    required this.onTapSecondary,
    this.enablePrimaryColor = false,
    this.enableSecondaryColor = false,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
        color: AppColors.background,
        height: 56,
        child: Row(
          children: [
            if (enablePrimaryColor) ...[
              LabelButton.primary(
                label: labelPrimary,
                onPressed: onTapPrimary,
              ),
            ] else ...[
              LabelButton.heading(
                label: labelPrimary,
                onPressed: onTapPrimary,
              ),
            ],
            if (enableSecondaryColor) ...[
              LabelButton.primary(
                label: labelSecondary,
                onPressed: onTapSecondary,
              ),
            ] else ...[
              LabelButton.heading(
                label: labelSecondary,
                onPressed: onTapSecondary,
              ),
            ],
          ],
        ));
  }
}
