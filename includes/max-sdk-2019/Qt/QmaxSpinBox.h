
// Copyright (c) 2016 Autodesk, Inc.
// All rights reserved.
// 
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.

#pragma once

/** \brief 3ds Max QSpinBox classes. 
 * These classes can be used as a simple drop-in replacement of Qts QSpinBox or
 * QDoubleSpinBox classes - to provide some additional features to the end-user
 * as dragging the spinner buttons to continuously change the value, cancelling
 * that and also reset by right mouse-button click on the buttons of the actual
 * SpinBox control. 
 * Most of the additional properties exposed by these classes get automatically
 * wired up on a PB2 param, when getting set up by the P_AUTO_UI_QT paramblock
 * definition. 
 *
 *  - min gets wired up to minimum().
 *  - max gets wired up to maximum().
 *  - scale gets wired up to singleStep() - so the scale is actually equals to 
 *    one click on a plus/minus button.
 */

#include "CoreExport.h"
#include <memory>

#pragma warning( push ) 
#pragma warning( disable: 4127 4251 4275 4800 ) 
#include <QtCore/qglobal.h>
#include <QtWidgets/qspinbox.h>
#pragma warning( pop )

#ifndef MAX_COMPONENTS
#include "../ILightingUnits.h"
#endif

class QMenu;

//-----------------------------------------------------------------------------
namespace MaxSDK {
//-----------------------------------------------------------------------------

template<typename T,class S> class QmaxSpinBoxPrivate;

class CoreExport QmaxDoubleSpinBox: public QDoubleSpinBox
{	
	Q_OBJECT

	/** \brief The reset value of this Component.
	 * \see resetValue(), setResetValue( double ) */
	Q_PROPERTY( double resetValue READ resetValue WRITE setResetValue )

	/** \brief Show the animation key brackets around the component.
	 * \see hasAnimationKeyBrackets(), setAnimationKeyBrackets() */
	Q_PROPERTY( bool animationKeyBrackets READ hasAnimationKeyBrackets WRITE setAnimationKeyBrackets )

	// working, but don't change the UI for now - just here for extensibility
	Q_PROPERTY( bool indeterminate READ isIndeterminate WRITE setIndeterminate )

	/** \brief Determines whether this component is in interactive mode.
	 * \see isInteractive(), interactiveChanged() */
	Q_PROPERTY( bool interactive READ isInteractive NOTIFY interactiveChanged )

	/** \property QmaxDoubleSpinBox::visibleDigits
     * \brief The number of digits used for the calculation of the minimum 
     * width of the component - values from -127 to +127 are valid, the 
     * default value is 6.
     * \see visibleDigits(), setVisibleDigits(), visibleDigitsChanged() */
    Q_PROPERTY( int visibleDigits READ visibleDigits WRITE setVisibleDigits NOTIFY visibleDigitsChanged )

public:

	explicit QmaxDoubleSpinBox( QWidget* parent = nullptr );
	virtual ~QmaxDoubleSpinBox();

	/** \brief The reset value of this component.
	 * If a user right-clicks the button-area of a QmaxSpinBox, it will be 
	 * reset to this value.
	 * \return The reset - aka default - value of this component.
	 * \see reset(), resetValue(), setResetValue( double ) */
	double resetValue() const;

	/** \brief Determines if the animation key brackets around the component 
	 *         should be shown. 
	 * \note The whole animationKeyBrackets property normally gets driven 
	 *       internally by a param-block mapper, so if this component is bound 
	 *       to a param-block, it may change automatically while scrubbing 
	 *       through the time-line, etc. 
	 * \return True, if the animation key brackets should be shown.
	 * \see setAnimationKeyBrackets() */
	bool hasAnimationKeyBrackets() const;

	// working, but don't change the UI for now - just here for extensibility
	bool isIndeterminate() const;
	
	/** \brief Determines whether this component is in interactive mode.
	 * A Component is in interactive mode if a user is changing its value in a 
	 * continuous manner, that may be cancelled by the user at the end of the 
	 * interaction. 
	 * For the spinner this is while dragging the buttons to change the value 
	 * or by holding down the left mouse-button over a button to increment
	 * or decrement the value over time. 
	 * 
	 * The basic idea of having an interactive mode is to enable the undo/redo
	 * system to track those interactive continuous changes as ONE single change
	 * (or even just ignores the change on cancel).
	 * 
	 * Therefore the interactiveChanged signal that gets emitted as soon as the 
	 * interactive mode changes has an additional param that is only meant to 
	 * be meaningful on ending an interactive session: the userCancelled param.
	 * \see interactiveChanged(), interactiveChanged */
	bool isInteractive() const;

    /** \brief The number of digits (before the period and the decimals) used
     * for the calculation of the minimum width of the component. The actual 
     * displayed number of digits may vary.
     *
     * The calculation of the minimum width is done in this way:
     *
     * Qt internally calculates the widths of the renderings of the minimum and
     * maximum values of the QSpinbox using its font, as well as - if set - the
     * specialValueText property to determine the actual width needed to show
     * all possible values.
     * This value is used, if visibleDigits is set to 0.
     *
     * Then the width of a value containing the absolute of visibleDigits digits,
     * like "222222.333" for 6 visibleDigits (and 3 decimals), is calculated
     * using the widget's font, including prefix and suffix.
     * If the visibleDigits is greater than 0, this value is compared against
     * the prior calculation result and the minimum of both is used - meaning 
     * the QmaxSpin box will reserve *up to* visibleDigits space, but nor more.
     *
     * If the visibleDigits is a negative number, the result of this calculation
     * is used, regardless of the fact that it might be able to display all its
     * values in a smaller space. This may be helpful to align different spin-
     * boxes more precisely.
     *
     * The default value for 3ds Max is 6 - but can be changed by the user with
     * this entry in the \c 3dsmax.ini file :
     *
     * \code
     * [UI]
     * QmaxSpinBox::visibleDigits=5
     * \endcode
     *
     * \see visibleDigits, setVisibleDigits(), visibleDigitsChanged() */
    int visibleDigits() const;

	virtual QValidator::State validate( QString& input, int& pos ) const override;
    virtual void stepBy(int steps) override;

    virtual QSize sizeHint() const override;
    virtual QSize minimumSizeHint() const override;

Q_SIGNALS:

	/** \brief Gets emitted as soon the component's interactive mode changes.
	 * \param interactive	The new interactive mode of the component.
	 * \param userCancelled	True if the user actively cancelled an interactive
	 *		session, for example by pressing the right mouse-button during a 
	 *		interactive dragging-operation. As a matter of fact, this param is
	 *		only meaningful if the interactive param was false.
	 * \see interactiveChanged(), interactiveChanged */
	void interactiveChanged( bool interactive, bool userCancelled = false );

    /** \brief Gets emitted when the context menu is shown.
    * \param contextMenu    A menu in which custom actions can be added. */
    void contextMenuCustomization( QMenu& contextMenu );

    /** \brief Gets emitted when the visibleDigits have been changed.
     * \param visibleDigits  The new value of the visibleDigits property. 
     * \see QmaxDoubleSpinBox::visibleDigits, setVisibleDigits(), visibleDigits() */
    void visibleDigitsChanged( int visibleDigits );

public Q_SLOTS:

	/** \brief Changes the reset value of this component.
	 * \param[in] resetValue The new resetValue
	 * \see reset(), resetValue() */
	void setResetValue( double resetValue );

	/** \brief Sets whether the animation key brackets around the component 
	 * should be shown.
	 * \note The whole animationKeyBrackets property normally gets driven
	 *		internally by a param-block mapper, so if this component it bound
	 *		to a param-block, it may change automatically while scrubbing
	 *		through the time-line, etc.
	 * \see hasAnimationKeyBrackets() */
	void setAnimationKeyBrackets( bool animationKeyBrackets = true );
	
	void setIndeterminate( bool indeterminate );

    /** \brief Sets the visibleDigits property.
     * Will emit a visibleDigitsChanged( visibleDigits ) signal, if the property
     * was changed.
     * \see QmaxDoubleSpinBox::visibleDigits, visibleDigits(), visibleDigitsChanged */
    void setVisibleDigits( int visibleDigits );

	/** \brief Resets this components value to its resetValue.
	 * \see setResetValue(), resetValue() */
	void reset();

protected:

	virtual void mousePressEvent( QMouseEvent* event ) override;
	virtual void mouseMoveEvent( QMouseEvent* event ) override;
	virtual void mouseReleaseEvent( QMouseEvent* event ) override;
	virtual void timerEvent( QTimerEvent* event ) override;
	virtual void paintEvent( QPaintEvent* event ) override;
	virtual void contextMenuEvent( QContextMenuEvent *event ) override; 
	virtual void keyPressEvent( QKeyEvent* event ) override;

	void setInteractive( bool interactive, bool userCanceled = false );

	virtual QString textFromValue( double val ) const override; 
	virtual double valueFromText( const QString& text ) const override;

protected:

	explicit QmaxDoubleSpinBox( QmaxSpinBoxPrivate<double,QmaxDoubleSpinBox>* d, QWidget* parent = nullptr );

	inline QmaxSpinBoxPrivate<double, QmaxDoubleSpinBox>* d_func() { return d_ptr; }
	inline const QmaxSpinBoxPrivate<double, QmaxDoubleSpinBox>* d_func() const { return d_ptr; }
	
	QmaxSpinBoxPrivate<double,QmaxDoubleSpinBox>* d_ptr;
	
	friend class QmaxSpinBoxPrivate<double, QmaxDoubleSpinBox>;
};

//-----------------------------------------------------------------------------
// QmaxUniverseSpinBox - a special QmaxDoubleSpinBox that displays current units
//-----------------------------------------------------------------------------
class CoreExport QmaxWorldSpinBox : public QmaxDoubleSpinBox {
	Q_OBJECT

public:

	explicit QmaxWorldSpinBox( QWidget* parent = nullptr );

    virtual QString textFromValue( double val ) const override;
    virtual double valueFromText( const QString& text ) const override;

protected:
	virtual void fixup( QString &str ) const override;
};



//-----------------------------------------------------------------------------
// class QmaxLightingUnitSpinBox
//-----------------------------------------------------------------------------

class QmaxLightingSpinBoxPrivate;
/** \brief a special QmaxSpinBox for displaying lighting units.
* \see QmaxDoubleSpinBox
*/
class CoreExport QmaxLightingSpinBox : public QmaxDoubleSpinBox {
	Q_OBJECT

	// The type of quantity being represented by this spin box
	Q_PROPERTY( QuantityType quantityType READ quantityType WRITE setQuantityType )

	// The unit system in which the internal value is represented (i.e. what's stored in the 'value' property).
	// The spin box will convert from this system to the display system when displaying the value.
	Q_PROPERTY( UnitSystem internalUnitSystem READ internalUnitSystem WRITE setInternalUnitSystem )

	Q_ENUMS( QuantityType UnitSystem )

public:

	// The type of quantity being represented by this spin box
	enum QuantityType {
		Luminance,
		Illuminance
	};
	#ifndef MAX_COMPONENTS
		typedef ILightingUnits::LightingSystems UnitSystem;
	#else
		enum UnitSystem {
			DISPLAY_INTERNATIONAL,  //! The SI system used cd?m� and lux
			DISPLAY_AMERICAN		//! The AS system uses cd?f� and footcandles
		};
	#endif

	explicit QmaxLightingSpinBox( QWidget* parent = nullptr );

	QuantityType quantityType() const;
	void setQuantityType( const QuantityType val );

	UnitSystem internalUnitSystem() const;
	void setInternalUnitSystem( const UnitSystem val );

	virtual double valueFromText( const QString &text ) const override;
	virtual QString textFromValue( double val ) const override;
	virtual void fixup( QString &str ) const override;

private:

	Q_DECLARE_PRIVATE_D( QmaxDoubleSpinBox::d_func(), QmaxLightingSpinBox );
};


//-----------------------------------------------------------------------------
// QmaxSpinBox - an integer based version of max special behaviour QSpinBox 
//-----------------------------------------------------------------------------
class CoreExport QmaxSpinBox : public QSpinBox {
	Q_OBJECT

		/** \brief The reset value of this Component.
		* \see resetValue(), setResetValue( double ) */
		Q_PROPERTY( int resetValue READ resetValue WRITE setResetValue )

		/** \brief Show the animation key brackets around the component.
		* \see hasAnimationKeyBrackets(), setAnimationKeyBrackets() */
		Q_PROPERTY( bool animationKeyBrackets READ hasAnimationKeyBrackets WRITE setAnimationKeyBrackets )

		// working, but don't change the UI for now - just here for extensibility
		Q_PROPERTY( bool indeterminate READ isIndeterminate WRITE setIndeterminate )

		/** \brief Determines whether this component is in interactive mode.
		* \see isInteractive(), interactiveChanged() */
		Q_PROPERTY( bool interactive READ isInteractive NOTIFY interactiveChanged )

        /** \property QmaxSpinBox::visibleDigits
         * \brief The number of digits used for the calculation of the minimum 
         * width of the component - values from -127 to +127 are valid, the 
         * default value is 6.
         * \see visibleDigits(), setVisibleDigits(), visibleDigitsChanged() */
        Q_PROPERTY( int visibleDigits READ visibleDigits WRITE setVisibleDigits NOTIFY visibleDigitsChanged )

public:

	explicit QmaxSpinBox( QWidget* parent = nullptr );
	virtual ~QmaxSpinBox();

	/** \brief The reset value of this component.
	 * If a user right-clicks the button-area of a QmaxSpinBox, it will be
	 * reset to this value.
	 * \return The reset - aka default - value of this component.
	 * \see reset(), resetValue(), setResetValue( int ) */
	int resetValue() const;

	/** \brief Determines if the animation key brackets around the component
	 *         should be shown.
	 * \note The whole animationKeyBrackets property normally gets driven
	 *       internally by a param-block mapper, so if this component it bound
	 *       to a param-block, it may change automatically while scrubbing
	 *       through the time-line, etc.
	 * \return True, if the animation key brackets should be shown.
	 * \see setAnimationKeyBrackets() */
	bool hasAnimationKeyBrackets() const;

	// working, but don't change the UI for now - just here for extensibility
	bool isIndeterminate() const;

	/** \brief Determines whether this component is in interactive mode.
	 * A Component is in interactive mode if a user is changing its value in a
	 * continuous manner, that may be cancelled by the user at the end of the
	 * interaction.
	 * For the spinner this is while dragging the buttons to change the value 
	 * or by holding down the left mouse-button over a button to increment
	 * or decrement the value over time.
	 *
	 * The basic idea of having an interactive mode is to enable the undo/redo
	 * system to track those interactive continuous changes as ONE single change
	 * (or even just ignores the change on cancel).
	 *
	 * Therefore the interactiveChanged signal that gets emitted as soon as the
	 * interactive mode changes has an additional param that is only meant to
	 * be meaningful on ending an interactive session: the userCancelled param.
	 * \see interactiveChanged(), interactiveChanged */
	bool isInteractive() const;

    /** \property QmaxSpinBox::visibleDigits
     * \brief The number of digits that is used for the calculation of the
     * minimum width of the component. The actual displayed number of digits may
     * vary.
     *
     * The calculation of the minimum width is done in this way:
     *
     * Qt internally calculates the widths of the renderings of the minimum and
     * maximum values of the QSpinbox using its font, as well as - if set - the
     * specialValueText property to determine the actual width needed to show
     * all possible values.
     * This value is used, if visibleDigits is set to 0.
     *
     * Then the width of a value containing the absolute of visibleDigits digits,
     * like "222222" for 6 visibleDigits, is calculated using the widget's
     * font, including prefix and suffix.
     * If the visibleDigits is greater than 0, this value is compared against
     * the prior calculation result and the minimum of both is used - meaning 
     * the QmaxSpin box will reserve *up to* visibleDigits space, but nor more.
     *
     * If the visibleDigits is a negative number, the result of this calculation
     * is used, regardless of the fact that it might be able to display all its
     * values in a smaller space. This may be helpful to align different spin-
     * boxes more precisely.
     *
     * The default value for 3ds Max is 6 - but can be changed by the user with
     * this entry in the \c 3dsmax.ini file :
     *
     * \code
     * [UI]
     * QmaxSpinBox::visibleDigits=5
     * \endcode
     *
     * \see visibleDigits, setVisibleDigits(), visibleDigitsChanged() */
    int visibleDigits() const;

	virtual QValidator::State validate( QString& input, int& pos ) const override;
    virtual void stepBy(int steps) override;

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

Q_SIGNALS:

	/** \brief Gets emitted as soon the components interactive mode changes.
	* \param interactive	The new interactive mode of the component.
	* \param userCancelled	True if the user actively cancelled an interactive
	*		session, for example by pressing the right mouse-button during a
	*		interactive dragging-operation. As a matter of fact, this param is
	*		only meaningful if the interactive param was false.
	* \see interactiveChanged(), interactiveChanged */
	void interactiveChanged( bool interactive, bool userCancelled = false );

	/** \brief Gets emitted when the context menu is shown.
	* \param contextMenu	A menu in which custom actions can be added. */
	void contextMenuCustomization(QMenu& contextMenu);

    /** \brief Gets emitted when the visibleDigits have been changed.
    * \param visibleDigits  The new value of the visibleDigits property. 
    * \see QmaxSpinBox::visibleDigits, setVisibleDigits(), visibleDigits() */
    void visibleDigitsChanged( int visibleDigits );

public Q_SLOTS:

	/** \brief Changes the reset value of this component.
	* \param[in] resetValue the new resetValue
	* \see reset(), resetValue() */
	void setResetValue( int resetValue );

	/** \brief Sets whether the animation key brackets around the component
	 * should be shown.
	 * \note The whole animationKeyBrackets property normally gets driven
	 *		internally by a param-block mapper, so if this component it bound
	 *		to a param-block, it may change automatically while scrubbing
	 *		through the time-line, etc.
	 * \see hasAnimationKeyBrackets() */
	void setAnimationKeyBrackets( bool animationKeyBrackets = true );

	void setIndeterminate( bool indeterminate );

    /** \property QmaxSpinBox::visibleDigits
     * \brief Sets the visibleDigits property.
     * Will emit a visibleDigitsChanged( visibleDigits ) signal, if the property
     * was changed.
     * \see QmaxSpinBox::visibleDigits, visibleDigits(), visibleDigitsChanged */
    void setVisibleDigits( int visibleDigits );

	/** \brief Resets this components value to its resetValue.
	* \see setResetValue(), resetValue() */
	void reset();

protected:

	virtual void mousePressEvent( QMouseEvent* event ) override;

	virtual void mouseMoveEvent( QMouseEvent* event ) override;
	virtual void mouseReleaseEvent( QMouseEvent* event ) override;
	virtual void timerEvent( QTimerEvent* event ) override;
	virtual void paintEvent( QPaintEvent* event ) override;
	virtual void contextMenuEvent( QContextMenuEvent *event ) override;
	virtual void keyPressEvent( QKeyEvent* event ) override;

	void setInteractive( bool interactive, bool userCanceled = false );

	virtual int valueFromText( const QString &text ) const override;

protected:

	explicit QmaxSpinBox( QmaxSpinBoxPrivate<int, QmaxSpinBox>* d, QWidget* parent = nullptr );

	inline QmaxSpinBoxPrivate<int, QmaxSpinBox>* d_func() { return d_ptr; }
	inline const QmaxSpinBoxPrivate<int, QmaxSpinBox>* d_func() const { return d_ptr; }

	QmaxSpinBoxPrivate<int, QmaxSpinBox>* d_ptr;

	friend class QmaxSpinBoxPrivate<int, QmaxSpinBox>;
};

//-----------------------------------------------------------------------------
} // end namespace MaxSDK
//-----------------------------------------------------------------------------
